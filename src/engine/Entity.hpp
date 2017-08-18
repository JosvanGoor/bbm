#ifndef ENGINE_ENTITY_HPP
#define ENGINE_ENTITY_HPP

#include <string>
#include "../core/Stringable.hpp"
#include "../geometry/Point.hpp"
#include "../geometry/Rectangle.hpp"
#include "../math/Matrix4x4.hpp"

namespace engine
{   
    class Entity : public Stringable
    {
        public:
            Entity(int type);
            virtual ~Entity() { };

            //getters
            int type() const;
            geometry::Pointf speed() const;
            geometry::Rectanglef position() const;
            bool marked_for_deletion() const;

            //setters
            void speed(float dx, float dy);
            void speed(const geometry::Pointf &sp);
            void position(float x, float y);
            void position(const geometry::Pointf &pos);
            void position(const geometry::Rectanglef &pos);
            void mark_for_deletion();
            
            virtual void act(); //progresses Entity 1 tick
            virtual void draw(double ahead); //draws entity (ahead is number of ticks since last actual update.)
            virtual void collision(Entity *entity); //manage a confirmed collision.
            
            virtual std::string to_string() const override;

        protected:
            int m_type; //actor type, for use with collision checking etc.
            std::string m_texture;
            bool m_marked_for_deletion;

            geometry::Pointf m_speed;
            geometry::Rectanglef m_position;
    };
}

#endif