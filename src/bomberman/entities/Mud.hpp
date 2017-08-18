#ifndef BOMBERMAN_ENTITIES_MUD_HPP
#define BOMBERMAN_ENTITIES_MUD_HPP

#include "../../engine/Entity.hpp"

class Mud : public engine::Entity
{
    public:
        Mud(const std::string &texture, const geometry::Rectanglef &pos);

        //destroy on collision, chance for powerup.
        virtual void collision(Entity *entity) override;

        virtual std::string to_string() const override;
};

#endif