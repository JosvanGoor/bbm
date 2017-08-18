#include "Bomb.hpp"

#include "../Bomberman.hpp"

Bomb::Bomb(int player, int power, const geometry::Rectanglef &position) : engine::Entity(BOMB)
{
    m_player = player;
    m_power = power;
    m_timer = BOMB_MAX_TIMER;
    m_position = position;
    m_texture = "textures/bomb_time_100.png";
}

int Bomb::player_id() const
{
    return m_player;
}

void Bomb::act()
{
    m_timer--;
    switch(m_timer)
    {
        case (BOMB_MAX_TIMER/2):
            m_texture = "textures/bomb_time_50.png";
            break;
        case (BOMB_MAX_TIMER/4):
            m_texture = "textures/bomb_time_25.png";
            break;
        case (BOMB_MAX_TIMER/20)+6:
            m_texture = "textures/bomb_time_5.png";
            break;
        case 6:
            m_texture = "textures/bomb_time_2.png";
            break;
        case 3:
            m_texture = "textures/bomb_time_1.png";
            break;
        case 0:
            boom();
            mark_for_deletion();
            break;
    }
}

std::string Bomb::to_string() const
{
    return "Bomb" + std::to_string(m_timer) + "/" + std::to_string(BOMB_MAX_TIMER) + " @ " + m_position.to_string();
}

void Bomb::boom()
{
    Level *level = (Level*)Bomberman::instance().game_state_controller();
    level->add_actor(new Explosion(m_player, m_position));

    //up
    geometry::Rectanglef pos(m_position);
    for(int i = 0; i <= m_power; ++i)
    {
        pos.translate(0.0f, -48.0f);
        if(level->collides_with_scenery(pos)) break;
        if(level->collides_with_mud(pos))
        {
            level->add_actor(new Explosion(m_player, pos));
            break;
        }
        level->add_actor(new Explosion(m_player, pos));
    }

    //down
    pos = m_position;
    for(int i = 0; i <= m_power; ++i)
    {
        pos.translate(0.0f, 48.0f);
        if(level->collides_with_scenery(pos)) break;
        if(level->collides_with_mud(pos))
        {
            level->add_actor(new Explosion(m_player, pos));
            break;
        }
        level->add_actor(new Explosion(m_player, pos));
    }

    //left
    pos = m_position;
    for(int i = 0; i <= m_power; ++i)
    {
        pos.translate(-48.0f, 0.0f);
        if(level->collides_with_scenery(pos)) break;
        if(level->collides_with_mud(pos))
        {
            level->add_actor(new Explosion(m_player, pos));
            break;
        }
        level->add_actor(new Explosion(m_player, pos));
    }

    //right
    pos = m_position;
    for(int i = 0; i <= m_power; ++i)
    {
        pos.translate(48, 0);
        if(level->collides_with_scenery(pos)) break;
        if(level->collides_with_mud(pos))
        {
            level->add_actor(new Explosion(m_player, pos));
            break;
        }
        level->add_actor(new Explosion(m_player, pos));
    }

    level->debug_print_actors();
}