#ifndef BOMBERMAN_BOMBERMAN_HPP
#define BOMBERMAN_BOMBERMAN_HPP

#include <chrono>
#include <random>
#include <SDL2/SDL.h>

#include "../engine/Controllers.hpp"
#include "../engine/DrawCall.hpp"
#include "../engine/Font.hpp"
#include "../engine/GameStateController.hpp"
#include "../engine/MouseController.hpp"
#include "../engine/RenderableString.hpp"
#include "../engine/ShaderProgram.hpp"
#include "../engine/TextureCache.hpp"
#include "../utility/Settings.hpp"

/*
    During setup / runtime some __values get set in the settings, 
    check below the class definition for a list.
*/

class Bomberman
{

    public:
        static Bomberman& instance() 
            { static Bomberman bomberman; return bomberman; }

        void game_loop(); //MAGIX

        //Getters:
        size_t tick_rate() const;
        size_t current_tick() const;
        engine::DrawCall& unity_quad(); //meant to be used with complex draw.
        engine::Font& default_font();
        engine::TextureCache& texture_cache();
        utility::Settings& settings();
        math::Matrix4f projection() const;
        math::Matrix4f default_view() const;
        engine::GameStateController* game_state_controller();
        engine::MouseController& mouse();
        engine::KeyboardController& keyboard_controller();
        engine::GamepadController& gamepad_controller_1();
        engine::GamepadController& gamepad_controller_2();
        engine::GamepadController& gamepad_controller_3();
        engine::GamepadController& gamepad_controller_4();
        engine::Controller& get_named_controller(engine::NamedController nc);
        engine::ShaderProgram* shader_program() const;
        GLuint shloc_texture() const;
        GLuint shloc_translation() const;
        GLuint shloc_view() const;
        GLuint shloc_projection() const;
        GLuint shloc_color_filter() const;

        //limits are including (code in accesors file)
        int random_int_number(int low, int high);
        double random_real_number(double low = 0.0, double high = 1.0);

        void quit(); //ends the gameloop.

    private:
        Bomberman(); //private because singleton
        ~Bomberman();

        //setup functions
        void opengl_setup();
        void shader_setup();
        void load_default_settings();

        //event handling
        void handle_events();

        //singleton!!
        Bomberman(const Bomberman&) = delete;
        Bomberman(Bomberman&&) = delete;
        void operator=(const Bomberman&) = delete;
        void operator=(Bomberman&&) = delete;

        //speed settings
        size_t m_tick_rate;
        size_t m_current_engine_tick;
        bool m_game_running;

        //Shared data structures
        //engine::DrawCall m_unity_quad;
        engine::DrawCall *m_unity_quad; //quad from (0,0) to (1,1) (both V&T).
        engine::Font *m_default_font;
        engine::TextureCache m_texture_cache;
        utility::Settings m_settings;
        math::Matrix4f m_projection;
        math::Matrix4f m_default_view;
        engine::GameStateController *m_game_state;

        //random numbers
        std::mt19937 m_int_random;
        std::mt19937 m_real_random;
        std::uniform_int_distribution<int> m_int_dist;
        std::uniform_real_distribution<double> m_real_dist;

        //user controls handles.
        engine::MouseController m_mouse;
        engine::KeyboardController m_keyboard;
        engine::GamepadController m_gamepad_1;
        engine::GamepadController m_gamepad_2;
        engine::GamepadController m_gamepad_3;
        engine::GamepadController m_gamepad_4;
        
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

/*
    On startup:
        __scr_w - width of the render area
        __scr_h - height of the render area
        __legacy - if this value is 1 the shaders use a version pre 4.4.
*/

enum EntityType
{
    PLAYER_1,   //TODO: players
    PLAYER_2,
    PLAYER_3,
    PLAYER_4,

    WALL,
    MUD,

    BOMB,       //TODO: bomb
    EXPLOSION,  //TODO: explosion

    POWERUP_AMMO,
    POWERUP_POWER,
    POWERUP_SPEED,
    POWERUP_POISON
};

#endif