#include "Controllers.hpp"

#include "Bomberman.hpp"

namespace engine
{

    //////////////////////////////////////////////////////////////////
    // Controller
    //////////////////////////////////////////////////////////////////

    Controller::Controller()
    {
        m_type = 0;
        m_rt = 0;
        m_lt = 0;
        m_connected = false;
        for(int i = 0; i < 14; ++i) m_buttons[i] = false;
        m_main_stick_x = 0;
        m_main_stick_y = 0;
        m_second_stick_x = 0;
        m_second_stick_y = 0;
    }

    char Controller::type() const { return m_type; }
    bool Controller::connected() const { return m_connected; }

    double Controller::main_stick_x() const { return m_main_stick_x; }
    double Controller::main_stick_y() const { return m_main_stick_y; }
    double Controller::second_stick_x() const { return m_second_stick_x; }
    double Controller::second_stick_y() const { return m_second_stick_y; }

    double Controller::rt() const { return m_rt; }
    double Controller::lt() const { return m_lt; }

    bool Controller::dpad_up() const { return m_buttons[DPAD_UP]; }
    bool Controller::dpad_down() const { return m_buttons[DPAD_DOWN]; }
    bool Controller::dpad_left() const { return m_buttons[DPAD_LEFT]; }
    bool Controller::dpad_right() const { return m_buttons[DPAD_RIGHT]; }
    bool Controller::start() const { return m_buttons[START]; }
    bool Controller::menu() const { return m_buttons[MENU]; }
    bool Controller::x() const { return m_buttons[X]; }
    bool Controller::y() const { return m_buttons[Y]; }
    bool Controller::a() const { return m_buttons[A]; }
    bool Controller::b() const { return m_buttons[B]; }
    bool Controller::rb() const { return m_buttons[RB]; }
    bool Controller::lb() const { return m_buttons[LB]; }
    bool Controller::rs() const { return m_buttons[RS]; }
    bool Controller::ls() const { return m_buttons[LS]; }

    //////////////////////////////////////////////////////////////////
    // Gamepad
    //////////////////////////////////////////////////////////////////

    GamepadController::GamepadController(SDL_JoystickID id)
        : Controller()
    {
        m_type = 'g';
        m_jid = id;
        m_connected = true;
        m_deadzone = Bomberman::instance().settings().as_double("controller_deadzone");
    }

    void GamepadController::update(const SDL_Event &event)
    {
        switch(event.type)
        {
            case SDL_CONTROLLERAXISMOTION:
                //todo implement
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                //todo implement
                break;
            case SDL_CONTROLLERBUTTONUP:
                //todo implement
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                //todo implement
                break;
        }
    }

    //////////////////////////////////////////////////////////////////
    // Keyboard
    //////////////////////////////////////////////////////////////////

    KeyboardController::KeyboardController()
        : Controller()
    {
        m_type = 'k';
        //map buttons
        m_mapping[DPAD_UP]      = SDLK_KP_8;
        m_mapping[DPAD_DOWN]    = SDLK_KP_5;
        m_mapping[DPAD_LEFT]    = SDLK_KP_4;
        m_mapping[DPAD_RIGHT]   = SDLK_KP_6;
        m_mapping[START]        = SDLK_RETURN;
        m_mapping[MENU]         = SDLK_ESCAPE;
        m_mapping[X]            = SDLK_RCTRL;
        m_mapping[Y]            = SDLK_RALT;
        m_mapping[A]            = SDLK_SPACE;
        m_mapping[B]            = SDLK_x;
        m_mapping[RB]           = SDLK_q;
        m_mapping[LB]           = SDLK_p;
        m_mapping[RS]           = SDLK_e;
        m_mapping[LS]           = SDLK_r;
        m_mapping[RT]           = SDLK_y;
        m_mapping[LT]           = SDLK_u;
        m_mapping[MAIN_UP]      = SDLK_UP;
        m_mapping[MAIN_DOWN]    = SDLK_DOWN;
        m_mapping[MAIN_LEFT]    = SDLK_LEFT;
        m_mapping[MAIN_RIGHT]   = SDLK_RIGHT;
        m_mapping[SECOND_UP]    = SDLK_w;
        m_mapping[SECOND_DOWN]  = SDLK_s;
        m_mapping[SECOND_LEFT]  = SDLK_a;
        m_mapping[SECOND_RIGHT] = SDLK_d;
    }

    void KeyboardController::update(const SDL_Event &event)
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                //todo implement
                break;
            case SDL_KEYUP:
                //todo implement
                break;
        }
    }
}