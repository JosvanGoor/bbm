#ifndef BOMBERMAN_ENTITIES_BOMB_HPP
#define BOMBERMAN_ENTITIES_BOMB_HPP

#include "../../engine/Entity.hpp"

#define BOMB_MAX_TIMER 200

class Bomb : public engine::Entity
{
    public:
        Bomb(int player, const geometry::Rectanglef &position);

        int player_id() const;
        virtual void act() const override; //tick down & explode.
    
    private:
        int m_timer; //tick tock boom
        int m_player; //player who dropped it.
};

#endif