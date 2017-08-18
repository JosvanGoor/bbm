#ifndef BOMBERMAN_ENTITIES_WALL_HPP
#define BOMBERMAN_ENTITIES_WALL_HPP

#include "../Bomberman.hpp"
#include "../../engine/Entity.hpp"
#include "../../geometry/Point.hpp"

class Wall : public engine::Entity
{
    public:
        Wall(const geometry::Rectanglef &position, const std::string &tex = "textures/brick.png");

        virtual std::string to_string() const override;
};

#endif