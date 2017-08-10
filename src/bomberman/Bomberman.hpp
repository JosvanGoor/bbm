#ifndef BOMBERMAN_BOMBERMAN_HPP
#define BOMBERMAN_BOMBERMAN_HPP

#include <chrono>
#include <SDL2/SDL.h>

#include "../engine/DrawCall.hpp"
#include "../engine/Font.hpp"
#include "../engine/RenderableString.hpp"
#include "../engine/ShaderProgram.hpp"
#include "../engine/TextureCache.hpp"
#include "../utility/Settings.hpp"

class Bomberman
{

    public:
        static Bomberman& instance() 
            { static Bomberman bomberman; return bomberman; }

        void game_loop(); //MAGIX

        //Getters:
        size_t tick_rate();
        size_t current_tick();
        engine::Font* default_font();
        engine::TextureCache& texture_cache();
        utility::Settings& settings();
        math::Matrix4f projection();
        math::Matrix4f default_view();
        engine::ShaderProgram* shader_program();
        GLuint shloc_texture();
        GLuint shloc_translation();
        GLuint shloc_view();
        GLuint shloc_projection();
        GLuint shloc_color_filter();

    private:
        Bomberman(); //private because singleton
        ~Bomberman();

        //setup functions
        void opengl_setup();
        void shader_setup();
        void load_default_settings();

        //singleton!!
        Bomberman(const Bomberman&) = delete;
        Bomberman(Bomberman&&) = delete;
        void operator=(const Bomberman&) = delete;
        void operator=(Bomberman&&) = delete;

        //speed settings
        size_t m_tick_rate;
        size_t m_current_engine_tick;

        //Shared data structures
        //engine::DrawCall m_unity_quad;
        engine::Font *m_default_font;
        engine::TextureCache m_texture_cache;
        utility::Settings m_settings;
        math::Matrix4f m_projection;
        math::Matrix4f m_default_view;

        //single-pass shader
        engine::ShaderProgram *m_shader_program;
        GLuint m_shloc_texture;
        GLuint m_shloc_translation; //model
        GLuint m_shloc_view; //view
        GLuint m_shloc_projection; //projection
        GLuint m_shloc_color_filter;

        //system handles
        SDL_Window *m_window_handle;
        SDL_GLContext m_opengl_context;
    
};

#endif