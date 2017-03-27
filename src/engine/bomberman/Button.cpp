#include "Button.hpp"

#include "../Bomberman.hpp"

namespace engine
{

    Button::Button(const std::string &str, int x, int y, int w, int h)
     : Actor() {
        m_texture = Bomberman::instance().texture_cache().get_texture(str);
        m_position = Rectangle(x, y, w,h);
    }

    bool Button::clicked(const Point &p)
    {
        return m_position.contains(p);
    }

}
