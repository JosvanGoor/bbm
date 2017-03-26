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
        m_position = Rectangle(0, 0, 1, 1);
    }

    int Actor::type() const { return m_type; }
    float Actor::speed_x() const { return m_speed_x; }
    float Actor::speed_y() const { return m_speed_y; }
    void Actor::speed_x(float f) { m_speed_x = f; }
    void Actor::speed_y(float f) { m_speed_y = f; }
    void Actor::set_speed(float x, float y)
    {
        m_speed_x = x;
        m_speed_y = y;
    }

    Rectangle& Actor::position() { return m_position; }
    void Actor::position(const Rectangle &rect) { m_position = rect; }

    void Actor::act()
    {
        m_position.translate(m_speed_x, m_speed_y);
    }

    void Actor::draw(double ahead)
    {
        Bomberman::instance().draw_quad(m_position, m_texture);
    }

    void Actor::collision(const Actor *act)
    {
        return; //nozink
    }

}