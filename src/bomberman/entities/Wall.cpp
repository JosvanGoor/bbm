#include "Wall.hpp"

Wall::Wall(const std::string &tex, const geometry::Rectanglef &pos) : engine::Entity(WALL)
{
    m_texture = tex;
    m_position = pos;
}

std::string Wall::to_string() const
{
    return "Wall @ " + m_position.to_string();
}