#ifndef ENGINE_BOMBERMAN_MAINMENU_HPP
#define ENGINE_BOMBERMAN_MAINMENU_HPP

#include "Button.hpp"
#include "../Room.hpp"
#include "../Bomberman.hpp"
#include "../../geometry/Point.hpp"

using geometry::Point;

namespace engine
{

    class MainMenu : public Room
    {
    public:
        MainMenu();
        ~MainMenu();

        virtual Room* logic_update();
        virtual void process_events();
        virtual void draw(double ahead);
    protected:
        Point m_last_click;
        Button *m_start;
        Button *m_quit;
    };

}



#endif