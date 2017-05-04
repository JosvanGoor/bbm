#include "Scenery.hpp"

#include "../../engine/Bomberman.hpp"

namespace engine
{

    Brick::Brick()
    {
        m_type = BRICK;
        m_texture = Bomberman::instance().texture_cache().get_texture("textures/brick.png");
        m_speed_x = 0;
        m_speed_y = 0;
        m_position = geometry::Rectangle(0, 0, 64, 64);
        m_marked_for_deletion = false;
    }

}