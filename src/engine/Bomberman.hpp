#ifndef BBM_BOMBERMAN_HPP
#define BBM_BOMBERMAN_HPP
#include "../extern/gl_core_4_4.h"

#include <map>
#include <string>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "TextureCache.hpp"
#include "ShaderProgram.hpp"
#include "../math/Matrix4x4.hpp"
#include "../utility/Settings.hpp"

using utility::Settings;

namespace engine
{

    /*
        This is a singleton class that runs the main gameloop
        and contains all state variables
    */

    class Bomberman
    {
    public:
        static Bomberman& instance()
        {
            static Bomberman bomberman;
            return bomberman;
        }

        //Calling this function will run the game, when the function returns the game is finished (or crashed =( )
        void game_loop(); //THIS IS WHERE THE MAGIC HAPPENS BAYBEEEEEEE

        //convenience functions
        void end_game();
        void push_trigger(const std::string &key, const std::string &message);
        std::pair<size_t, std::string> get_recent_trigger(std::string key, size_t maxdelay);

        //fuckton of setters/getters
        bool legacy_mode() const;
        bool game_loop_running() const;
        size_t tick_rate() const;
        double tick_duration() const;
        size_t start_room_tick() const;
        size_t current_engine_tick() const;

        Settings& settings();
        TextureCache& texture_cache();
        
        //returns [0, ""] when no such key can be found
        std::map<std::string, std::pair<size_t, std::string>>& triggers();

        GLuint shloc_projection() const;
        GLuint shloc_translation() const;
        GLuint shloc_texture() const;

        size_t grid_width() const;
        size_t grid_heigt() const;

    private:
        //initializer functions
        void windowing_setup();
        void shader_setup();

        //some mode-flags
        bool m_legacy_mode;
        bool m_game_loop_running;

        //speed info
        size_t m_tick_rate;
        double m_tick_duration;
        size_t m_current_engine_tick;
        size_t m_start_room_tick;

        //caches
        Settings m_settings;
        TextureCache m_texture_cache;
        std::map<std::string, std::pair<size_t, std::string>> m_triggers;

        //projection & shaders
        ShaderProgram *m_shader_program;
        GLuint m_shloc_projection;
        GLuint m_shloc_translation;
        GLuint m_shloc_texture;

        math::Matrix4x4<float> m_projection;
        size_t m_grid_width;
        size_t m_grid_height;

        //System handles
        SDL_Window *m_window_handle;
        SDL_GLContext m_opengl_context;

        //SDL_Window *window = nullptr;
        //SDL_GLContext opengl_context;


        //constructors & copy prevention
        Bomberman();
        ~Bomberman();

        //setting that lists default settings.
        void load_default_settings();

        Bomberman(const Bomberman&) = delete;
        Bomberman(const Bomberman&&) = delete;
        void operator=(const Bomberman&) = delete;
        void operator=(Bomberman&&) = delete;
    };

}

#endif
