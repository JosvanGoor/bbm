#include "Destructables.hpp"

#include "../../engine/Bomberman.hpp"

namespace engine
{

    Mud::Mud()
    {
        m_type = MUD;
        m_texture = Bomberman::instance().texture_cache().get_texture("textures/mud.png");
        m_speed_x = 0;
        m_speed_y = 0;
        m_position = geometry::Rectangle(0, 0, 64, 64);
        m_marked_for_deletion = false;
    }

}