#ifndef BBM_BOMBERMAN_HPP
#define BBM_BOMBERMAN_HPP
#include "../extern/gl_core_4_4.h"

#include <map>
#include <string>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Font.hpp"
#include "Room.hpp"
#include "Controllers.hpp"
#include "TextureCache.hpp"
#include "ShaderProgram.hpp"
#include "../math/Math.hpp"
#include "../math/Matrix4x4.hpp"
#include "../bomberman/MainMenu.hpp"
#include "../utility/Settings.hpp"
#include "../geometry/Rectangle.hpp"

using utility::Settings;

namespace engine
{
    /*
        This enum allows referring to a certain enumerated controller
        ie. gamepad2 = gamepad0+2
        they are tightly packed so keyboard_1 = gamepad_4 + 1.
        --> for(i = GAMEPAD_1; i <= KEYBOARD_1; ++i) enumerates all controllers.
    */
    enum NamedController
    {
        UNCONNECTED = 0,
        GAMEPAD_1 = 1,
        GAMEPAD_2 = 2,
        GAMEPAD_3 = 3,
        GAMEPAD_4 = 4,
        KEYBOARD_1 = 5,
        KEYBOARD_2 = 6
    };

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
        //returns [0, ""] when no such key can be found
        std::pair<size_t, std::string> get_recent_trigger(std::string key, size_t maxdelay);
        void draw_quad(const geometry::Rectangle &pos, GLuint texture, int offset_x = 0, int offset_y = 0);

        Controller *controller(NamedController nc); //returns selected controller.

        //input sink (should receive ALL keyboard and gamecontroller events)
        void event_sink(const SDL_Event &event); //manages events room doesnt want to deal with

        //fuckton of setters/getters
        bool legacy_mode() const;
        bool game_loop_running() const;
        size_t tick_rate() const;
        double tick_duration() const;
        size_t last_fps() const;
        size_t last_lps() const;
        size_t start_room_tick() const;
        size_t current_engine_tick() const;

        Font* font();
        Settings& settings();
        TextureCache& texture_cache();
        
        std::map<std::string, std::pair<size_t, std::string>>& triggers();

        GLuint quad_vao() const;
        GLuint quad_vbo() const;

        GLuint shloc_projection() const;
        GLuint shloc_translation() const;
        GLuint shloc_texture() const;
        GLuint shloc_color_filter() const;

        math::Matrix4x4<float> projection() const;

        size_t drawspace_width() const;
        size_t drawspace_height() const;

    private:
        //current running room
        Room *m_active_room;

        //initializer functions
        void windowing_setup();
        void shader_setup();
        void basic_draw_setup(); //setup quad vao/vbo

        //some mode-flags
        bool m_legacy_mode;
        bool m_game_loop_running;

        //speed info
        size_t m_tick_rate;
        double m_tick_duration;
        size_t m_last_fps; //frames previous second
        size_t m_last_lps; //logic updates previous second
        size_t m_current_engine_tick;
        size_t m_start_room_tick;
        
        //caches
        Font *m_font;
        Settings m_settings;
        TextureCache m_texture_cache;
        std::map<std::string, std::pair<size_t, std::string>> m_triggers;

        //Controller links
        KeyboardController *m_keyboard_1;
        GamepadController *m_gamepad_1;
        GamepadController *m_gamepad_2;
        GamepadController *m_gamepad_3;
        GamepadController *m_gamepad_4;

        //projection & shaders
        ShaderProgram *m_shader_program;
        GLuint m_shloc_texture;
        GLuint m_shloc_projection;
        GLuint m_shloc_translation;
        GLuint m_shloc_color_filter;

        //drawing stuff
        GLuint m_quad_vao;
        GLuint m_quad_vbo;
        math::Matrix4x4<float> m_transform;
        math::Matrix4x4<float> m_projection;
        
        //draw grid size //drawspace rename?
        size_t m_drawspace_width;
        size_t m_drawspace_height;

        //System handles
        SDL_Window *m_window_handle;
        SDL_GLContext m_opengl_context;

        //setting that lists default settings.
        void load_default_settings();

        //constructors & copy prevention
        Bomberman();
        ~Bomberman();

        Bomberman(const Bomberman&) = delete;
        Bomberman(const Bomberman&&) = delete;
        void operator=(const Bomberman&) = delete;
        void operator=(Bomberman&&) = delete;
    };

}

#endif
