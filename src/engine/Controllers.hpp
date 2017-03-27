#ifndef ENGINE_CONTROLLERS_HPP
#define ENGINE_CONTROLLERS_HPP

#include <SDL2/SDL.h>

namespace engine
{

    /*
        BOOLEAN ARRAY INDICES
    */
    enum ControllerKey
    {
        DPAD_UP = 0,
        DPAD_DOWN = 1,
        DPAD_LEFT = 2,
        DPAD_RIGHT = 3,
        START = 4,
        MENU = 5,
        X = 6,
        Y = 7,
        A = 8,
        B = 9,
        RB = 10,
        LB = 11,
        RS = 12,
        LS = 13,
        RT = 14, //from this space its only for button mapping (keyboardcontroller)
        LT = 15,
        MAIN_UP = 16,
        MAIN_DOWN = 17,
        MAIN_LEFT = 18,
        MAIN_RIGHT = 19,
        SECOND_UP = 20,
        SECOND_DOWN = 21,
        SECOND_LEFT = 22,
        SECOND_RIGHT = 23,
    };

    /*
        Controller layout
        There are 3 types (sofar):
            0: unknown
            'k': keyboard
            'g': gamepad
    */
    class Controller
    {
    public:
        Controller();
        virtual ~Controller() { };

        char type() const; //returns controller type
        bool connected() const; //returns false when the controller lost connection.
        virtual void update(const SDL_Event &event) = 0; //updates state

        //[-1.0, 1.0]
        double main_stick_x() const;
        double main_stick_y() const;
        double second_stick_x() const;
        double second_stick_y() const;

        //[0.0, 1.0]
        double rt() const; //right shoulder trigger
        double lt() const; //left shoulder trigger

        //[true | false]
        bool dpad_up() const;
        bool dpad_down() const;
        bool dpad_left() const;
        bool dpad_right() const;

        bool start() const;
        bool menu() const;

        bool x() const;
        bool y() const;
        bool a() const;
        bool b() const;
        bool rb() const; //right shoulder button
        bool lb() const; //left shoulder button
        bool rs() const; //right stick button
        bool ls() const; //left stick button

    protected:
        //state variables.
        char m_type; //controller type. 0 = none, ''
        double m_rt;
        double m_lt;
        bool m_connected;
        bool m_buttons[14];
        double m_main_stick_x;
        double m_main_stick_y;
        double m_second_stick_x;
        double m_second_stick_y;
    };

    class GamepadController : public Controller
    {
    public:
        GamepadController(SDL_JoystickID id);
        ~GamepadController() { };

        virtual void update(const SDL_Event &event);

    protected:
        double m_deadzone;
        SDL_JoystickID m_jid;
    };

    class KeyboardController : public Controller
    {
    public:
        KeyboardController(); //only default mapping atm.

        virtual void update(const SDL_Event &event);
    protected:
        SDL_Keycode m_mapping[24];
    };

}

#endif