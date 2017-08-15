#include "Wall.hpp"

Wall::Wall(const std::string &tex, const geometry::Rectanglef &pos) : engine::Entity(WALL)
{
    m_texture = tex;
    m_marked_for_deletion = false;

    m_speed = geometry::Pointf(0.0f, 0.0f);
    m_position = pos;
}