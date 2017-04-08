#ifndef ENGINE_BOMBERMAN_BUTTON_HPP
#define ENGINE_BOMBERMAN_BUTTON_HPP

#include "../Actor.hpp"
#include "../../geometry/Point.hpp"

using geometry::Point;

namespace engine
{

    class Button : public Actor
    {
    public:
        Button(const std::string &tex, int x, int y, int w, int h);
        virtual ~Button() {};
        
        bool clicked(const Point &p);
    };

}

#endif