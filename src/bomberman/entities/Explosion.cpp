#include "Explosion.hpp"

#include "../Bomberman.hpp"

Explosion::Explosion(int player,const geometry::Rectanglef &position) : engine::Entity(EXPLOSION)
{
    m_player = player;
    m_timer = 40;
    m_texture = "textures/explosion.png";
    m_position = position;
}

int Explosion::player_id() const
{
    return m_player;
}

void Explosion::act()
{
    m_timer--;
    if(m_timer == 0) mark_for_deletion();
}

std::string Explosion::to_string() const
{
    return "Explosion @ " + m_position.to_string();
}