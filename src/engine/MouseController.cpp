#include "MouseController.hpp"

#include <iostream>

namespace engine
{

    MouseController::MouseController()
    {
        m_cursor_x = 0;
        m_cursor_y = 0;
        m_dx = 0;
        m_dy = 0;
        m_scrolled = 0;
    }

    void MouseController::tick()
    {
        m_dx = m_dy = m_scrolled = 0;
        if(m_lmb == BUTTON_RELEASED) m_lmb = BUTTON_UP;
        if(m_lmb == BUTTON_PRESSED) m_lmb = BUTTON_DOWN;
        if(m_mmb == BUTTON_RELEASED) m_mmb = BUTTON_UP;
        if(m_mmb == BUTTON_PRESSED) m_mmb = BUTTON_DOWN;
        if(m_rmb == BUTTON_RELEASED) m_rmb = BUTTON_UP;
        if(m_rmb == BUTTON_PRESSED) m_rmb = BUTTON_DOWN;
    }

    void MouseController::update(const SDL_Event &event)
    {
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                m_cursor_x = event.motion.x;
                m_cursor_y = event.motion.y;
                m_dx += event.motion.xrel;
                m_dy += event.motion.yrel;
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        m_lmb = BUTTON_PRESSED;
                        break;
                    case SDL_BUTTON_MIDDLE:
                        m_mmb = BUTTON_PRESSED;
                        break;
                    case SDL_BUTTON_RIGHT:
                        m_rmb = BUTTON_PRESSED;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        m_lmb = BUTTON_RELEASED;
                        break;
                    case SDL_BUTTON_MIDDLE:
                        m_mmb = BUTTON_RELEASED;
                        break;
                    case SDL_BUTTON_RIGHT:
                        m_rmb = BUTTON_RELEASED;
                        break;
                }
                break;
            case SDL_MOUSEWHEEL:
                m_scrolled += event.wheel.y;
                break;
        }
    }
    
    ButtonState MouseController::left_mouse_button() const
    {
        return m_lmb;
    }

    ButtonState MouseController::middle_mouse_button() const
    {
        return m_mmb;
    }

    ButtonState MouseController::right_mouse_button() const
    {
        return m_rmb;
    }

    int MouseController::scroll_distance() const
    {
        return m_scrolled;
    }

    geometry::Point<int> MouseController::cursor_position() const
    {
        return geometry::Point<int>(m_cursor_x, m_cursor_y);
    }

    geometry::Point<int> MouseController::previous_cursor_position() const
    {
        return geometry::Point<int>(m_cursor_x - m_dx, m_cursor_y - m_dy);
    }

    int MouseController::cursor_x() const
    {
        return m_cursor_x;
    }

    int MouseController::cursor_y() const
    {
        return m_cursor_y;
    }

    int MouseController::cursor_dx() const
    {
        return m_dx;
    }

    int MouseController::cursor_dy() const
    {
        return m_dy;
    }

    std::string MouseController::to_string() const
    {
        std::string s = "engine::Mouse - @ [" + std::to_string(m_cursor_x) + ", " + std::to_string(m_cursor_y) + "]";
        return s;
    }

}