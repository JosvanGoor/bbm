#include <cstdio>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "math/Vector3.hpp"
#include "extern/gl_core_4_4.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace math;

std::string translate_opengl_error_code(GLenum error)
{
    switch(error)
    {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        default: return "UNKNOWN_ERROR";
    }
}

std::string translate_last_opengl_error()
{
    return translate_opengl_error_code(glGetError());
}

void printIfError(std::string msg)
{
    GLenum err = glGetError();
    if(err == GL_NO_ERROR) return;
    std::cout << msg << " - " << translate_opengl_error_code(err) << std::endl;
}

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

    //shader code
    GLuint shader_program = glCreateProgram();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vsh = readFile("shaders/vertexshader2d.glsl");
    const char* ccvsh = vsh.c_str(); int svsh = vsh.size();
    std::string fsh = readFile("shaders/fragmentshader2d.glsl");
    const char* ccfsh = fsh.c_str(); int sfsh = fsh.size();

    glShaderSource(vertex_shader, 1, &ccvsh, &svsh);
    glShaderSource(fragment_shader, 1, &ccfsh, &sfsh);

    glCompileShader(vertex_shader);
    GLint success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        GLchar info[1024];
        glGetShaderInfoLog(vertex_shader, sizeof(info), NULL, info);
        std::cout << info << std::endl;
        return -1337;
    }

    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        GLchar info[1024];
        glGetShaderInfoLog(fragment_shader, sizeof(info), NULL, info);
        std::cout << info << std::endl;
        return -1337;
    }

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLchar info[1024];
        glGetProgramInfoLog(shader_program, sizeof(info), NULL, info);
        std::cout << info << std::endl;
        return -13337;
    }

    glUseProgram(shader_program);

    //vertices
    std::vector<float> floats = {-1.0, -1.0, 0.0,
                                 1.0, -1.0, 0.0,
                                 0.0, 1.0, 0.0};
    
    //build vbo
    GLuint vao;
    glGenVertexArrays(1, &vao);
    printIfError("glGenVertexArrays");
    glBindVertexArray(vao);
    printIfError("glBindVertexArray");

    GLuint vbo;
    glGenBuffers(1, &vbo);
    printIfError("glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    printIfError("glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, floats.size() * sizeof(float), floats.data(), GL_STATIC_DRAW);
    printIfError("glBufferData");


    while(!quit)
    {

        glEnableVertexAttribArray(0);
        printIfError("glEnableVertexAttribArray");
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        printIfError("glBindBuffer");
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        printIfError("glVertexAttribPointer");
        glDrawArrays(GL_TRIANGLES, 0, 3);
        printIfError("glDrawArrays");
        glDisableVertexAttribArray(0);
        printIfError("glDisableVertexAttribArray");

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