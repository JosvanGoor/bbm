#include "Mud.hpp"

#include <random>

#include "Powerup.hpp"
#include "../Bomberman.hpp"
#include "../Level.hpp"

Mud::Mud(const geometry::Rectanglef &position, const std::string &tex) : engine::Entity(MUD)
{
    m_texture = tex;
    m_position = position;
}

void Mud::collision(engine::Entity *entity)
{
    if(entity->type() == EXPLOSION)
    {
        mark_for_deletion();

        Level *level = (Level*)Bomberman::instance().game_state_controller();
        //TODO: FIX RANDOM GENERATOR BUGS WHY IS IT SO WEIRD.
        switch(rand() % 9)
        {
            case 0:
                level->add_actor(new Powerup(POWERUP_AMMO, m_position));
                break;
            case 1:
                level->add_actor(new Powerup(POWERUP_POWER, m_position));
                break;
            case 2:
                level->add_actor(new Powerup(POWERUP_SPEED, m_position));
                break;
            case 11:
                level->add_actor(new Powerup(POWERUP_POISON, m_position));
                break;
        }
    }
}

std::string Mud::to_string() const
{
    return "Mud @ " + m_position.to_string();
}