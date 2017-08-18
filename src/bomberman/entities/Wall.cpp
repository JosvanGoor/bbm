#include "Wall.hpp"

Wall::Wall(const geometry::Rectanglef &pos, const std::string &tex) : engine::Entity(WALL)
{
    m_texture = tex;
    m_position = pos;
}

std::string Wall::to_string() const
{
    return "Wall @ " + m_position.to_string();
}