#include "Event.hpp"

#include <windows.h>

namespace gui
{

    int Event::type() const { return m_type; }
    int Event::action() const { return m_action; }
    long Event::time() const { return m_time; }
    int Event::button() const { return m_button; }
    int Event::key() const { return m_button; }
    int Event::x() const { return m_x; }
    int Event::y() const { return m_y; }
    int Event::dx() const { return m_dx; }
    int Event::dy() const { return m_dy; }

    std::string Event::to_string() const
    {
        std::string s = "gui::Event - ";
        switch(m_type)
        {
            case BBM_NO_EVENT:
                s += "BBM_NO_EVENT\n"; break;
            case BBM_WINDOW_EVENT:
                s += "BBM_WINDOW_EVENT\n"; break;
            case BBM_KEYBOARD_EVENT:
                s += "BBM_KEYBOARD_EVENT\n"; break;
            case BBM_MOUSE_EVENT:
                s += "BBM_MOUSE_EVENT\n"; break;
            case BBM_MOUSEMOTION_EVENT:
                s += "BBM_MOUSEMOTION_EVENT\n"; break;
            default:
                s += "UNKNOWN_EVENT_TYPE\n"; break;
        }

        s += "    Action: ";
        switch(m_action) 
        {
            case BBM_MOUSEMOVED: s += "BBM_MOUSEMOVED\n"; break;
            case BBM_MOUSEDRAGGED: s += "BBM_MOUSEDRAGGED\n"; break;
            case BBM_MOUSEBUTTONDOWN: s += "BBM_MOUSEBUTTONDOWN\n"; break;
            case BBM_MOUSEBUTTONUP: s += "BBM_MOUSEBUTTONUP\n"; break;
            case BBM_KEYDOWN: s += "BBM_KEYDOWN\n"; break;
            case BBM_KEYUP: s += "BBM_KEYUP\n"; break;
            default: s += "UNKNOWN_ACTION_TYPE\n"; break;
        }

        s += "    Time: " + std::to_string(m_time);
        s += "    Button: " + std::to_string(m_button);
        s += "    x: " + std::to_string(m_x);
        s += "    y: " + std::to_string(m_y);
        s += "    dx: " + std::to_string(m_dx);
        s += "    dy: " + std::to_string(m_dy);
        return s;
    }

    int get_modifier_keys()
    {
        int flags = 0;

        if(GetKeyState(VK_LMENU)) flags |= BBM_MODIFIER_LALT | BBM_MODIFIER_ALT;
        if(GetKeyState(VK_RMENU)) flags |= BBM_MODIFIER_RALT | BBM_MODIFIER_ALT;
        if(GetKeyState(VK_LSHIFT)) flags |= BBM_MODIFIER_LSHIFT | BBM_MODIFIER_SHIFT;
        if(GetKeyState(VK_RSHIFT)) flags |= BBM_MODIFIER_RSHIFT | BBM_MODIFIER_SHIFT;
        if(GetKeyState(VK_LCONTROL)) flags |= BBM_MODIFIER_LCTRL | BBM_MODIFIER_CTRL;
        if(GetKeyState(VK_RCONTROL)) flags |= BBM_MODIFIER_RCTRL | BBM_MODIFIER_CTRL;

        return flags;
    }

}