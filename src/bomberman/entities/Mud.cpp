#include "Mud.hpp"

#include "../Bomberman.hpp"

Mud::Mud(const std::string &tex, const geometry::Rectanglef &position) : engine::Entity(MUD)
{
    m_texture = tex;
    m_position = position;
}

void Mud::collision(engine::Entity *entity)
{
    //TODO: Add collision to explosion
}