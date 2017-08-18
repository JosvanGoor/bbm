#ifndef BOMBERMAN_ENTITIES_BOMB_HPP
#define BOMBERMAN_ENTITIES_BOMB_HPP

#include "Explosion.hpp"
#include "../Level.hpp"
#include "../../engine/Entity.hpp"
#include "../../geometry/Rectangle.hpp"

#define BOMB_MAX_TIMER 120

class Bomb : public engine::Entity
{
    public:
        Bomb(int player, int power, const geometry::Rectanglef &position);

        int player_id() const;
        virtual void act() override; //tick down & explode.

        virtual std::string to_string() const override;
    
    private:
        int m_power;
        int m_timer; //tick tock boom
        int m_player; //player who dropped it.

        void boom();
};

#endif