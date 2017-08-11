#include "Entity.hpp"

#include "../bomberman/Bomberman.hpp"

namespace engine
{

    Entity::Entity(int type)
    {
        m_type = 0;
        m_texture = "";
        m_marked_for_deletion = false;
        m_speed = geometry::Pointf();
        m_position = geometry::Rectanglef();
    }

    int Entity::type() const { return m_type; }
    geometry::Pointf Entity::speed() const { return m_speed; }
    geometry::Rectanglef Entity::position() const { return m_position; }
    bool Entity::marked_for_deletion() const { return m_marked_for_deletion; }

    void Entity::speed(float dx, float dy) { m_speed.set(dx, dy); }
    void Entity::speed(const geometry::Pointf &sp) { m_speed.set(sp.x(), sp.y()); }
    void Entity::position(float x, float y) { m_position.position(x, y); }
    void Entity::position(const geometry::Pointf &pos) { m_position.position(pos.x(), pos.y()); }
    void Entity::position(const geometry::Rectanglef &pos) { m_position = pos; }
    void Entity::mark_for_deletion() { m_marked_for_deletion = true; }

    void Entity::act()
    {
        /* Default implementation: empty */
    }
    
    void Entity::draw(double ahead)
    {
        /* Default implementation: draw as simple quad */
        //math::Matrix4f tf;
        //tf = tf.scale(m_position.width(), m_position.height(), 0.0f);
        //tf = tf.translate(m_position.x(), m_position.y(), 0.0f);
        Bomberman::instance().unity_quad().draw(m_position, m_texture);
    }

    void Entity::collision(Entity *entity)
    {
        /* Default implementation: empty */
    }

    std::string Entity::to_string() const
    {
        std::string s = "engine::Entity of type ";
        s += std::to_string(m_type) += "\n";
        s += "    pos: " + m_position.to_string() + "\n";
        s += "    speed: " + m_speed.to_string() + "\n";
        s += "    marked for deletion: " + std::to_string(m_marked_for_deletion);
        return s;
    }
}