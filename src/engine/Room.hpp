#ifndef BBM_ENGINE_ROOM_HPP
#define BBM_ENGINE_ROOM_HPP

namespace engine
{

    /*
        The room class is a subclass to all "states" the game can be in,
        it can be then main menu, settings menu, player selectionscreen or 
        an actual ingame lvl.

        The update function returns a pointer to a new room, 
        when it is null nothing happens, when it is an actual pointer
        the engine swaps to that room and destroys the room that returned 
        said pointer, It is up to the room to make sure all engine values are
        set as needed.

        Collision checking is left to the implementation of the locic update function.

        The draw function gets passed a positive double value which describes
        the tickdurations that have passed since the last logic update. This value
        will usually be somewhere between 0 and 1, but it can be higher then 1
        when the engine can't keep up with realtime.
    */

    class Room
    {
    public:
        Room() { };
        virtual ~Room() { }

        virtual Room* logic_update() = 0;
        virtual void draw(double ahead) = 0;
    };


}

#endif