#ifndef BBM_GUI_EVENT_HPP
#define BBM_GUI_EVENT_HPP

#include "windows.h"
#include "../Core.hpp"

namespace gui
{

    enum
    {
        //event types
        BBM_NO_EVENT =              0x0000,
        BBM_WINDOW_EVENT =          0x0001,
        BBM_KEYBOARD_EVENT =        0x0002,
        BBM_MOUSE_EVENT =           0x0003,
        BBM_MOUSEMOTION_EVENT =     0x0004,

        //event action types
        BBM_MOUSEMOVED =            0x0101,
        BBM_MOUSEDRAGGED =          0x0102,
        BBM_MOUSEBUTTONUP =         0x0104,
        BBM_MOUSEBUTTONDOWN =       0x0103,
        BBM_KEYDOWN =               0x0105,
        BBM_KEYUP =                 0x0106,

        //window events
        BBM_WINDOW_MOVED =          0x0201,
        BBM_WINDOW_CLOSED =         0x0202,
        BBM_WINDOW_RESIZED =        0x0203,
        BBM_WINDOW_LOSTFOCUS =      0x0204,
        BBM_WINDOW_GAINEDFOCUS =    0x0205,

        //modifier key constants
        BBM_MODIFIER_ALT =          0b000000000001,
        BBM_MODIFIER_LALT =         0b000000000010,
        BBM_MODIFIER_RALT =         0b000000000100,
        BBM_MODIFIER_SHIFT =        0b000000001000,
        BBM_MODIFIER_RSHIFT =       0b000000010000,
        BBM_MODIFIER_LSHIFT =       0b000000100000,
        BBM_MODIFIER_CTRL =         0b000001000000,
        BBM_MODIFIER_LCTRL =        0b000010000000,
        BBM_MODIFIER_RCTRL =        0b000100000000,

        //passthrough messages
        BBM_PASS_WM_CLOSE =         WM_USER + 0x00,
        BBM_PASS_WM_MOVE =          WM_USER + 0x01,
        BBM_PASS_WM_ACTIVATEAPP =   WM_USER + 0x02
    };


    class Event : public Object
    {
    public:
        Event(int type = 0, int action = 0, long time = 0, int button = 0, int x = 0, int y = 0, int dx = 0, int dy = 0)
            : m_type(type), m_action(action), m_time(time), m_button(button), m_x(x), m_y(y), m_dx(dx), m_dy(dy) {}
            
        int type() const;
        int action() const;
        long time() const;
        int button() const;
        int key() const; //same as button
        int x() const;
        int y() const;
        int dx() const;
        int width() const; //same as dx
        int dy() const;
        int height() const; //same as dy

        //virtual override
        virtual std::string to_string() const;

    protected:
        int m_type; //event type
        int m_action; //performed action
        long m_time; //time at which event was generated

        int m_button; //which button is used
        int m_x, m_y, m_dx, m_dy; //coordinates when applicable
    };

    //returns modifier keys that are down when this function is called.
    int get_modifier_keys();
};

#endif
