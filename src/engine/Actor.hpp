#ifndef ENGINE_ACTOR_HPP
#define ENGINE_ACTOR_HPP

#include "../geometry/Rectangle.hpp"

namespace engine
{

    class Actor
    {
    public:
        Actor();
        virtual ~Actor() { };
        
        int type() const;        
        float speed_x() const;
        float speed_y() const;
        void speed_x(float f);
        void speed_y(float f);
        void set_speed(float x, float y);
        
        geometry::Rectangle &position();
        void position(const geometry::Rectangle &rect);

        virtual void act();
        virtual void draw(double ahead);
        virtual void collision(const Actor *act);

    protected:
        int m_type; //every actor needs an unique type
        GLuint m_texture;
        
        float m_speed_x;
        float m_speed_y;
        geometry::Rectangle m_position;
    };

}

#endif