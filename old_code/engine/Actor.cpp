#include "Actor.hpp"

#include "Bomberman.hpp"

namespace engine
{

    Actor::Actor()
    {
        m_type = 0;
        m_texture = Bomberman::instance().texture_cache().get_texture("textures/unknown.png");
        m_speed_x = 0;
        m_speed_y = 0;
        m_position = geometry::Rectangle(0, 0, 1, 1);
        m_marked_for_deletion = false;
    }

    int Actor::type() const { return m_type; }
    bool Actor::marked_for_deletion() const { return m_marked_for_deletion; }
    float Actor::speed_x() const { return m_speed_x; }
    float Actor::speed_y() const { return m_speed_y; }
    void Actor::speed_x(float f) { m_speed_x = f; }
    void Actor::speed_y(float f) { m_speed_y = f; }
    void Actor::set_speed(float x, float y)
    {
        m_speed_x = x;
        m_speed_y = y;
    }

    std::array<float, 8> Actor::texture_coordinates() const
    {
        return std::array<float, 8> 
                { 0.0, 0.0, //x y
                  1.0, 0.0, //x+w y
                  0.0, 1.0, //x y+h
                  1.0, 1.0 }; //x+w y+h
    }    
    geometry::Rectangle& Actor::position() { return m_position; }
    void Actor::position(const geometry::Rectangle &rect) { m_position = rect; }

    void Actor::act()
    {
        m_position.translate(m_speed_x, m_speed_y);
    }

    void Actor::draw(double ahead, int offset_x, int offset_y)
    {
        Bomberman::instance().draw_quad(m_position, m_texture, offset_x, offset_y);
    }

    void Actor::collision(const Actor *act)
    {
        return; //nozink
    }

}