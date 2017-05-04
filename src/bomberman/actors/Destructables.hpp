#ifndef ENGINE_BOMBERMAN_ACTORS_DESTRUCTABLES_HPP
#define ENGINE_BOMBERMAN_ACTORS_DESTRUCTABLES_HPP

#include "../../engine/Actor.hpp"

namespace engine 
{

    class Mud : public Actor
    {
    public:
        Mud();
        virtual ~Mud() { };

        //on destroy maybe powerup

    };

}

#endif