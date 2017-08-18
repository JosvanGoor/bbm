#ifndef BOMBERMAN_ENTITIES_EXPLOSION_HPP
#define BOMBERMAN_ENTITIES_EXPLOSION_HPP

#include "../../engine/Entity.hpp"
#include "../../geometry/Rectangle.hpp"

class Explosion : public engine::Entity
{
    public:
        Explosion(int player, const geometry::Rectanglef &position);

        int player_id() const;
        virtual void act() override;

        virtual std::string to_string() const override;

    private:
        int m_timer;
        int m_player;
};

#endif