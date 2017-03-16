#include "Engine.hpp"
#include "../extern/gl_core_4_4.h"

#include <map>
#include <chrono>
#include <string>
#include <vector>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "TextureCache.hpp"
#include "ShaderProgram.hpp"

namespace engine
{
    bool initialized = false;
    size_t tick_rate; //ticks/second
    size_t current_tick;

    SDL_Window *window_handle;
    SDL_GLContext opengl_context;

    TextureCache texture_cache;
    ShaderProgram *shader_program;
        GLuint shader_ortho_location;
        GLuint shader_translation_location;
        GLuint shader_texture_location;

    //triggers stored under key, with occured tick, value.
    std::map<std::string, std::vector<std::pair<size_t, std::string>>> triggers;

    ////////////////////////////////////////////////////////////////////////////////
    //                            initialize_engine                               //
    ////////////////////////////////////////////////////////////////////////////////

    void initialize_engine()
    {
        if(initialized) throw Exception(__PRETTY_FUNCTION__, "Engine already initialized");
        std::cout << "----- starting engine intialization -----\n";
        ///////////////////
        // TODO: add settings file.
        ///////////////////

        tick_rate = 40;

        if(SDL_Init(SDL_INIT_VIDEO) < 0) throw Exception(__PRETTY_FUNCTION__, "error initializing sdl libraries");

        window_handle = SDL_CreateWindow("Bomberman v0.1", 200, 200, 1366, 768, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        if(!window_handle) throw Exception(__PRETTY_FUNCTION__, "could not create sdl window");
        std::cout << "Created window...\n";

        opengl_context = SDL_GL_CreateContext(window_handle);
        if(!opengl_context) throw Exception(__PRETTY_FUNCTION__, "cound not create opengl context for sdl window");
        std::cout << "Set openGL4.4 context...\n";

        if(ogl_LoadFunctions() == ogl_LOAD_FAILED) throw Exception(__PRETTY_FUNCTION__, "failed to load opengl extentions (version 4.4 requested)");
        std::cout << "Loaded openGL4.4 functions and extentions\n";

        SDL_GL_SetSwapInterval(0); //disable vsync

        shader_program = ShaderProgram::create();
        try 
        {
            std::cout << "attempting to compile shaders... ";
            shader_program->add_vertex_shader("shaders/vertexshader2d.glsl");
            shader_program->add_fragment_shader("shaders/fragmentshader2d.glsl");
            std::cout << "done\n";
        }
        catch(Exception &exc)
        {
            std::cout << "failed: " << exc.message() << "\n";
            std::cout << "attempting to compile legacy shaders... ";
            shader_program->add_vertex_shader("shaders/vertexshader2dlegacy.glsl");
            shader_program->add_fragment_shader("shaders/fragmentshader2dlegacy.glsl");
            std::cout << "done\n";
        }
        shader_program->link();
        shader_program->activate();

        GLint shader_ortho_location = shader_program->uniform_location("ortho");
        GLint shader_texture_location = shader_program->uniform_location("tex");
        GLint shader_translation_location = shader_program->uniform_location("transform");
        std::cout << "shader_ortho_location: " << shader_ortho_location << "\n";
        std::cout << "shader_texture_location: " << shader_texture_location << "\n";
        std::cout << "shader_translation_location: " << shader_translation_location << "\n";

        std::cout << "Compiled shaders, linked and activated shaderprogram\n";

        ///////////////////
        // TODO: load initial room.
        ///////////////////

        initialized = true;
        std::cout << "----- finished engine intialization -----" << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //                                game_loop                                   //
    ////////////////////////////////////////////////////////////////////////////////

    void game_loop()
    {
        std::cout << "----- starting game loop -----\n";
        current_tick = 0;
        double time_between_ticks = 1000.0 / tick_rate;
        std::cout << "Time between ticks: " << time_between_ticks << "\n";

        auto current_time = std::chrono::high_resolution_clock::now();
        auto previous_time = std::chrono::high_resolution_clock::now();
        double lag = 0.0;

        bool running = true; //false = quit.

        double in_this_second = 0.0;
        size_t updates = 0;
        size_t render_passes = 0;

        std::cout << "Ended preparations, entering loop...\n";
        //start gameloop
        while(running)
        {
            current_time = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(current_time - previous_time).count();
            lag += elapsed;
            previous_time = current_time;

            in_this_second += elapsed;
            if(in_this_second >= 1000)
            {
                std::cout << "updates last second: " << updates << std::endl;
                std::cout << "render passes last second: " << render_passes << std::endl;
                updates = render_passes = 0;
                in_this_second -= 1000;
            }

            /////////////////////////////////////// PROCESS INPUTS /////////////////////////////////////////
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    running = false;
                }
            }


            while(lag >= time_between_ticks)
            {
                /////////////////////////////////////// LOGIC UPDATES /////////////////////////////////////////
                
                /////////////////////////////////////// COLLISION UPDATES /////////////////////////////////////////
                
                
                lag -= time_between_ticks;
                updates++;
                current_tick++;
            }

            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //----- rendering code -----



            //----- rendering code -----

            SDL_GL_SwapWindow(window_handle);

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            render_passes++;
        }


        std::cout << "----- finished gameloop, bye bye :-) -----" << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //                           deinitialize_engine                              //
    ////////////////////////////////////////////////////////////////////////////////

    void unload_engine()
    {
        std::cout << "----- started unloading engine -----\n";

        texture_cache.clear();
        std::cout << "cleared all textures from memory\n";

        delete shader_program;
        std::cout << "prepared shaderprogram for deconstruction\n";

        SDL_GL_DeleteContext(opengl_context);
        SDL_DestroyWindow(window_handle);
        SDL_Quit();
        std::cout << "Destroyed sdl window, and quit environment\n";
        
        std::cout << "----- finished unloading engine -----" << std::endl;
    }

}