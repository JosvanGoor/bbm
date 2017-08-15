#ifndef ENGINE_MOUSECONTROLLER_HPP
#define ENGINE_MOUSECONTROLLER_HPP

#include <SDL2/SDL.h>
#include "../core/Stringable.hpp"
#include "../geometry/Point.hpp"

namespace engine
{

    enum ButtonState
    {
        BUTTON_UP,// = 0b10, //not pressed
        BUTTON_RELEASED,// = 0b11, //pressed last tick, released now
        BUTTON_DOWN,// = 0b001, //pressed
        BUTTON_PRESSED// = 0b0011 //released last tick, pressed now
    };

    /*
        Keeps a representation of the mouse/cusor state.
        void tick() resets the dx/dy/scrolled variables.
    */
    class MouseController : public Stringable
    {
        public:
            MouseController();

            void tick(); //resets state, should be called after the logic update.
            void update(const SDL_Event &event); //called by engine to update state.

            ButtonState left_mouse_button() const;
            ButtonState middle_mouse_button() const;
            ButtonState right_mouse_button() const;

            int scroll_distance() const;
            geometry::Point<int> cursor_position() const;
            geometry::Point<int> previous_cursor_position() const;

            int cursor_x() const;
            int cursor_y() const;
            int cursor_dx() const;
            int cursor_dy() const;

            virtual std::string to_string() const override;

        private:
            ButtonState m_lmb;
            ButtonState m_mmb;
            ButtonState m_rmb;
            int m_cursor_x;
            int m_cursor_y;
            int m_dx;
            int m_dy;
            int m_scrolled; //+ for up, - for down.

    };

}

#endif