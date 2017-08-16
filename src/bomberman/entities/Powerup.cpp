#include "Powerup.hpp"

#include "../Bomberman.hpp"
#include "../../core/Exception.hpp"

Powerup::Powerup(int type, const geometry::Rectanglef &position) : engine::Entity(type)
{
    switch(type)
    {
        case POWERUP_AMMO:
            m_texture = "textures/powerup_ammo.png";
            break;
        case POWERUP_POISON:
            m_texture = "textures/powerup_poison.png";
            break;
        case POWERUP_POWER:
            m_texture = "textures/powerup_power.png";
            break;
        case POWERUP_SPEED:
            m_texture = "textures/powerup_speed.png";
            break;
        default:
            throw Exception(__PRETTY_FUNCTION__, "Failed to recognize appropriate powerup type for " + std::to_string(type));
    }
    m_position = position;
}

std::string Powerup::to_string() const
{
    return "Powerup " + std::to_string(m_type) + " @ " + m_position.to_string();
}