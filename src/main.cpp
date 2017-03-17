#include <cstdio>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Core.hpp"
#include "engine/Engine.hpp"
#include "engine/TextureCache.hpp"
#include "engine/ShaderProgram.hpp"
#include "geometry/Rectangle.hpp"
#include "math/Vector3.hpp"
#include "math/Matrix4x4.hpp"
#include "extern/lodepng.h"
#include "extern/gl_core_4_4.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace math;
using namespace engine;
using namespace geometry;

std::string readFile(std::string file)
{
    std::ifstream t(file.c_str());
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    return str;
}

int main2(int argc, char **argv)
{

    SDL_Window *window = nullptr;
    SDL_GLContext opengl_context;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error intializing sdl" << std::endl;
        return -1;
    }

    //setup opengl version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    window = SDL_CreateWindow("SDL tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1366, 768, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == nullptr)
    {
        std::cout << "Could not create window" << std::endl;
        return -2;
    }

    opengl_context = SDL_GL_CreateContext(window);
    if(!opengl_context)
    {
        std::cout << "Could not create opengl context" << std::endl;
        return -3;
    }

    if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
    {
        std::cout << "Failed loading opengl 4.4" << std::endl;
        return -42;
    }

    SDL_Event event;
    bool quit = false;

    ShaderProgram *sp = ShaderProgram::create();
    sp->add_vertex_shader("shaders/vertexshader2d.glsl");
    sp->add_fragment_shader("shaders/fragmentshader2d.glsl");
    //sp->link();
    sp->activate();

    TextureCache cache;

    //load texture
    unsigned int width, height;
    std::vector<unsigned char> image;
    unsigned int error = lodepng::decode(image, width, height, std::string("textures/test_tex.png"));
    if(error != 0)
    {
        std::cout << "Error loading texture file" << std::endl;
        return -12378;
    }

    GLuint gltex;
    glGenTextures(1, &gltex);
    glBindTexture(GL_TEXTURE_2D, gltex);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST = no smoothing
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    print_if_opengl_error("glTexImage2D");

    Matrix4x4<float> translation;
    translation = translation.translate(200.0f, 200.0f, 0.0f);
    std::cout << "sizeof m4x4: " << sizeof(Matrix4x4<float>) << std::endl;
    Matrix4x4<float> ortho = orthographic_projection(0.0f, 1366.0f, 768.0f, .0f, -1.0f, 1.0f);
    GLint ortho_location = sp->uniform_location("ortho");
    GLuint translation_location = sp->uniform_location("transform");
    GLint texture_location = sp->uniform_location("tex");

    std::cout << "ortho_location: " << ortho_location << std::endl;
    std::cout << "translation_location: " << translation_location << std::endl;
    std::cout << "texture_location: " << texture_location << std::endl;

    float LOW = 10.01;
    float TEXLOW = 0.0;
    float TEXHGH = 1.0;
    float HGH = 700.99;
    std::vector<float> floats =  {LOW, LOW, 0.0, TEXLOW, TEXLOW,
                                  HGH, LOW, 0.0, TEXHGH, TEXLOW,
                                  LOW, HGH, 0.0, TEXLOW, TEXHGH,
                                  HGH, LOW, 0.0, TEXHGH, TEXLOW,
                                  HGH, HGH, 0.0, TEXHGH, TEXHGH,
                                  LOW, HGH, 0.0, TEXLOW, TEXHGH};

    //build vbo
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gltex);
    print_if_opengl_error("glBindTexture");

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, floats.size() * sizeof(float), floats.data(), GL_STATIC_DRAW);
    print_if_opengl_error("glBufferData");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    print_if_opengl_error("glEnableArrays");

    int stride = (3 + 2) * sizeof(float);
    void* vertex_offset = (void*)0;
    void* texture_offset = (void*)(3 * sizeof(float));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, vertex_offset);
    print_if_opengl_error("glVertexAttrib vertices");
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, texture_offset);
    print_if_opengl_error("glVertexAttrib texture");

    glUniformMatrix4fv(ortho_location, 1, GL_FALSE, ortho.data());
    glUniformMatrix4fv(translation_location, 1, GL_FALSE, translation.data());
    print_if_opengl_error("glUniformMatrix");

    while(!quit)
    {
        glBindTexture(GL_TEXTURE_2D, cache.get_texture("textures/test_tex.png"));
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        SDL_GL_SwapWindow(window);
        glClearColor(.0f, .0f, .0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                 quit = true;
                 break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int main(int argv, char **argc)
{
    std::string str("0000=1111");
    std::cout << "substr: " << str.substr(0, str.find_first_of("=")) << std::endl;
    
    try
    {
        engine::initialize_engine();
        std::cout << std::endl;

        engine::game_loop();
        std::cout << std::endl;
        
        engine::unload_engine();
    }catch(Exception &e)
    {
        std::cout << e.message() << std::endl;
    }

    return 0;
}