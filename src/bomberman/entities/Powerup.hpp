#ifndef BOMBERMAN_ENTITIES_POWERUP_HPP
#define BOMBERMAN_ENTITIES_POWERUP_HPP

#include "../../engine/Entity.hpp"

class Powerup : public engine::Entity
{
    public:
        Powerup(int type, const geometry::Rectanglef &position);

        virtual std::string to_string() const override;
};

#endif