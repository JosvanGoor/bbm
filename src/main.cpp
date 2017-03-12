#include <cstdio>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Core.hpp"
#include "engine/ShaderProgram.hpp"
#include "math/Vector3.hpp"
#include "extern/gl_core_4_4.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace math;
using namespace engine;

std::string readFile(std::string file)
{
    std::ifstream t(file.c_str());
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    return str;
}

int main(int argc, char **argv)
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
    if(!window)
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

    ShaderProgram *sp = new ShaderProgram();
    sp->add_vertex_shader("shaders/vertexshader2d.glsl");
    sp->add_fragment_shader("shaders/fragmentshader2d.glsl");
    sp->link();
    sp->activate();

    //vertices
    std::vector<float> floats = {-1.0, -1.0, 0.0,
                                 1.0, -1.0, 0.0,
                                 0.0, 1.0, 0.0};
    
    //build vbo
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, floats.size() * sizeof(float), floats.data(), GL_STATIC_DRAW);


    while(!quit)
    {

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        SDL_GL_SwapWindow(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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