#ifndef ENGINE_GAMESTATECONTROLLER_HPP
#define ENGINE_GAMESTATECONTROLLER_HPP

#include "../core/Stringable.hpp"

namespace engine
{

    /*
        A certain GameStateController determines what to draw to the screen,
        what logic updates should look like etc etc. This can range from the main menu,
        to player select, to actually playing a level.

        The update function returns a pointer to a new gamestate. If the pointer is null
        this gamestatecontroller will persist, when it is a pointer the engine
        will swap to that gamestate and destroy the current.

        The engine will make two distinct calls to subclasses: 
        logic_update() -> progress the gamestate 1 tick.
        draw_to_screen(float) -> draw the current state to the screen.

        The float passed to the draw function will usually be between 0.0 and 1.0,
        showing the ammount of ticks that have passed since the last logic_update().
    */

    class GameStateController : public Stringable
    {
        public:
            GameStateController() { };
            virtual ~GameStateController() { }; //empty deconstructor, default constructor

            //disable copying
            GameStateController(const GameStateController&) = delete;
            GameStateController(GameStateController&&) = delete;
            void operator=(const GameStateController&) = delete;
            void operator=(GameStateController&&) = delete;

            virtual GameStateController* logic_update() = 0; //abstract
            virtual void draw_to_screen(float ahead) = 0; //abstract

            virtual std::string to_string() const override;
    };

}

#endif