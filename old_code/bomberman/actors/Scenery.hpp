#ifndef BOMBERMAN_ENGINE_ACTORS_SCENERY_HPP
#define BOMBERMAN_ENGINE_ACTORS_SCENERY_HPP

#include "../../engine/Actor.hpp"

namespace engine
{

    class Brick : public Actor
    {
    public:
        Brick();
        virtual ~Brick() { };
    };

}

#endif