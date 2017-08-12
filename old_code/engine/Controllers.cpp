#include "Controllers.hpp"

#include <sstream>
#include <iomanip>

#include "../Core.hpp"
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
    int Controller::player() const { return m_player; }
    void Controller::player(int i) { m_player = i; }
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

    std::string Controller::state_to_string() const
    {
        if(!m_connected) return "Disconnected.";
        std::stringstream ss;
        ss.precision(2);

        ss << std::setw(3) <<"Sticks - Left: " << m_main_stick_x << "/" << m_main_stick_y << "|" << m_buttons[RS] << ", Right: " << m_second_stick_x << "/" << m_second_stick_y << "|" << m_buttons[LS];
        ss << ". Triggers - Left: " << m_lt << ", Right: " << m_rt;
        ss << ". Dpad - up: " << m_buttons[DPAD_UP] << " down: " << m_buttons[DPAD_DOWN] << " left: " << m_buttons[DPAD_LEFT] << " right: " << m_buttons[DPAD_RIGHT];
        ss << ". Start: " << m_buttons[START] << ", Menu: " << m_buttons[MENU] << ", Bumpers - left: " << m_buttons[LB] << ", right: " << m_buttons[RB];
        ss << ". A: " << m_buttons[A] << ", B: " << m_buttons[B] << ", X: " << m_buttons[X] << ", Y: " << m_buttons[Y];
        return ss.str();
    }

    //////////////////////////////////////////////////////////////////
    // Gamepad
    //////////////////////////////////////////////////////////////////

    GamepadController::GamepadController()
        : Controller()
    {
        m_type = 'g';
        m_jid = -1;
        m_controller = nullptr;
        m_connected = false;
        m_deadzone = 0.0;
    }

    GamepadController::GamepadController(SDL_GameController *controller)
        : Controller()
    {
        m_type = 'g';
        m_controller = controller;

        if(m_controller)
        {
            m_jid = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
            m_connected = true;
        }
        else
        {
            m_jid = -1;
            m_connected = false;
        }
        m_deadzone = Bomberman::instance().settings().as_double("controller_deadzone");
    }

    void GamepadController::update(const SDL_Event &event)
    {
        double newval;
        switch(event.type)
        {
            case SDL_CONTROLLERAXISMOTION:
                if(event.caxis.which != m_jid) return;
                
                newval = math::stretch(event.caxis.value, -MAX_CONTROLLER_RANGE, MAX_CONTROLLER_RANGE, -1, 1);
                if(std::abs(newval) < m_deadzone) newval = 0;

                switch(event.caxis.axis)
                {
                    case SDL_CONTROLLER_AXIS_LEFTX:
                        m_main_stick_x = newval;
                        break;
                    case SDL_CONTROLLER_AXIS_LEFTY:
                        m_main_stick_y = newval;
                        break;
                    case SDL_CONTROLLER_AXIS_RIGHTX:
                        m_second_stick_x = newval;
                        break;
                    case SDL_CONTROLLER_AXIS_RIGHTY:
                        m_second_stick_y = newval;
                        break;
                    case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                        m_rt = newval;
                        break;
                    case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                        m_lt = newval;
                        break;
                }
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                if(event.cbutton.which != m_jid) return;
                
                switch(event.cbutton.button)
                {
                    case SDL_CONTROLLER_BUTTON_A:
                        m_buttons[A] = true; break;
                    case SDL_CONTROLLER_BUTTON_B:
                        m_buttons[B] = true; break;
                    case SDL_CONTROLLER_BUTTON_X:
                        m_buttons[X] = true; break;
                    case SDL_CONTROLLER_BUTTON_Y:
                        m_buttons[Y] = true; break;
                    case SDL_CONTROLLER_BUTTON_BACK:
                        m_buttons[MENU] = true; break;
                    case SDL_CONTROLLER_BUTTON_START:
                        m_buttons[START] = true; break;
                    case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                        m_buttons[LS] = true; break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                        m_buttons[RS] = true; break;
                    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                        m_buttons[LB] = true; break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                        m_buttons[RB] = true; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        m_buttons[DPAD_UP] = true; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        m_buttons[DPAD_DOWN] = true; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                        m_buttons[DPAD_LEFT] = true; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        m_buttons[DPAD_RIGHT] = true; break;
                }

                break;
            case SDL_CONTROLLERBUTTONUP:
                if(event.cbutton.which != m_jid) return;
                
                switch(event.cbutton.button)
                {
                    case SDL_CONTROLLER_BUTTON_A:
                        m_buttons[A] = false; break;
                    case SDL_CONTROLLER_BUTTON_B:
                        m_buttons[B] = false; break;
                    case SDL_CONTROLLER_BUTTON_X:
                        m_buttons[X] = false; break;
                    case SDL_CONTROLLER_BUTTON_Y:
                        m_buttons[Y] = false; break;
                    case SDL_CONTROLLER_BUTTON_BACK:
                        m_buttons[MENU] = false; break;
                    case SDL_CONTROLLER_BUTTON_START:
                        m_buttons[START] = false; break;
                    case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                        m_buttons[LS] = false; break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                        m_buttons[RS] = false; break;
                    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                        m_buttons[LB] = false; break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                        m_buttons[RB] = false; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        m_buttons[DPAD_UP] = false; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        m_buttons[DPAD_DOWN] = false; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                        m_buttons[DPAD_LEFT] = false; break;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        m_buttons[DPAD_RIGHT] = false; break;
                }

                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                if(event.cdevice.which != m_jid) return;
                SDL_GameControllerClose(m_controller);
                m_controller = nullptr;
                m_connected = false;
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
        m_connected = true;
    }

    KeyboardController::KeyboardController(const std::string &mapping)
    {
        std::vector<std::string> maps = split(mapping, ',');
        if(maps.size() != 24) throw Exception(__PRETTY_FUNCTION__, "Expected mapping of 24 buttons got " + std::to_string(maps.size()) + " instead.");

        for(size_t i = 0; i < 24; ++i)
        {
            m_mapping[i] = std::stoi(maps[i]);
        }

        m_type = 'k';
        m_connected = true;
    }

    std::string KeyboardController::mapping() const
    {
        std::stringstream ss;

        ss << m_mapping[0];
        for(size_t i = 1; i < 24; ++i)
        {
            ss << "," << m_mapping[i];
        }

        return ss.str();
    }

    void KeyboardController::map_key(ControllerKey ck, SDL_Keycode kc)
    {
        m_mapping[ck] = kc;
    }

    void KeyboardController::update(const SDL_Event &event)
    {
        switch(event.key.type)
        {
            case SDL_KEYDOWN:
                //boolean buttons first
                for(int i = 0; i < 14; ++i)
                {
                    if(m_mapping[i] == event.key.keysym.sym)
                    {
                        m_buttons[i] = true;
                        break;
                    }
                }

                if(event.key.keysym.sym == m_mapping[RT])
                    m_rt = 1.0;
                else if(event.key.keysym.sym == m_mapping[LT])
                    m_lt = 1.0;
                else if(event.key.keysym.sym == m_mapping[MAIN_UP])
                    m_main_stick_y = 1.0;
                else if(event.key.keysym.sym == m_mapping[MAIN_DOWN])
                    m_main_stick_y = -1.0;
                else if(event.key.keysym.sym == m_mapping[MAIN_LEFT])
                    m_main_stick_x = -1.0;
                else if(event.key.keysym.sym == m_mapping[MAIN_RIGHT])
                    m_main_stick_x = 1.0;
                else if(event.key.keysym.sym == m_mapping[SECOND_UP])
                    m_second_stick_y = 1.0;
                else if(event.key.keysym.sym == m_mapping[SECOND_DOWN])
                    m_second_stick_y = -1.0;
                else if(event.key.keysym.sym == m_mapping[SECOND_LEFT])
                    m_second_stick_x = -1.0;
                else if(event.key.keysym.sym == m_mapping[SECOND_RIGHT])
                    m_second_stick_x = 1.0;
                break;
            case SDL_KEYUP:
                //boolean buttons first
                for(int i = 0; i < 14; ++i)
                {
                    if(m_mapping[i] == event.key.keysym.sym)
                    {
                        m_buttons[i] = false;
                        break;
                    }
                }

                if(event.key.keysym.sym == m_mapping[RT])
                    m_rt = 0.0;
                else if(event.key.keysym.sym == m_mapping[LT])
                    m_lt = 0.0;
                else if(event.key.keysym.sym == m_mapping[MAIN_UP])
                    m_main_stick_y = 0.0;
                else if(event.key.keysym.sym == m_mapping[MAIN_DOWN])
                    m_main_stick_y = 0.0;
                else if(event.key.keysym.sym == m_mapping[MAIN_LEFT])
                    m_main_stick_x = 0.0;
                else if(event.key.keysym.sym == m_mapping[MAIN_RIGHT])
                    m_main_stick_x = 0.0;
                else if(event.key.keysym.sym == m_mapping[SECOND_UP])
                    m_second_stick_y = 0.0;
                else if(event.key.keysym.sym == m_mapping[SECOND_DOWN])
                    m_second_stick_y = 0.0;
                else if(event.key.keysym.sym == m_mapping[SECOND_LEFT])
                    m_second_stick_x = 0.0;
                else if(event.key.keysym.sym == m_mapping[SECOND_RIGHT])
                    m_second_stick_x = 0.0;

                break;
        }
    }
}