#ifndef ENGINE_ACTOR_HPP
#define ENGINE_ACTOR_HPP

#include <array>
#include "../geometry/Rectangle.hpp"

namespace engine
{

    /*
        Lists the types of actors there are.
    */
    enum ActorTypes
    {
        /* (0, 100] Special actors */
        /* (100, 200] Scenery */
        BRICK = 100,

        /* (200, 300] Destructables */
        MUD = 200
    };

    class Actor
    {
    public:
        Actor();
        virtual ~Actor() { };
        
        int type() const;        
        bool marked_for_deletion() const;
        float speed_x() const;
        float speed_y() const;
        void speed_x(float f);
        void speed_y(float f);
        void set_speed(float x, float y);
        
        std::array<float, 8> texture_coordinates() const; //returns texcoords for objects on a spritesheet. [x,y xw,y x,yh xw,yh]
        geometry::Rectangle &position();
        void position(const geometry::Rectangle &rect);

        virtual void act();
        virtual void draw(double ahead, int offset_x = 0, int offset_y = 0);
        virtual void collision(const Actor *act);

    protected:
        int m_type; //every actor needs an unique type
        bool m_marked_for_deletion;
        GLuint m_texture;
        
        float m_speed_x;
        float m_speed_y;
        geometry::Rectangle m_position;
    };

}

#endif