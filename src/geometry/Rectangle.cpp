#include "Rectangle.hpp"

namespace geometry
{

    Rectangle::Rectangle(int x, int y, int w, int h)
    {
        m_x = x;
        m_y = y;
        m_width = w;
        m_height = h;
    }

    void Rectangle::translate(int dx, int dy)
    {
        m_x += dx;
        m_y += dy;
    }

    void Rectangle::set(int x, int y, int w, int h)
    {
        m_x = x;
        m_y = y;
        m_width = w;
        m_height = h;
    }

    bool Rectangle::contains(const Point &p)
    {
        if(p.x() < m_x || p.x() > (m_x + m_width)) return false;
        if(p.y() < m_y || p.y() > (m_y + m_height)) return false;
        return true;
    }

    bool Rectangle::intersects(const Rectangle &r)
    {
        if(r.x() > (m_x + m_width)) return false;
        if((r.x() + r.width()) < m_x) return false;

        if(r.y() > (m_y + m_height)) return false;
        if((r.y() + r.height()) < m_y) return false;

        return true;
    }

    int Rectangle::x() const { return m_x; }
    void Rectangle::x(int x) { m_x = x; } 
    int Rectangle::y() const { return m_y; }
    void Rectangle::y(int y) { m_y = y; }
    int Rectangle::width() const { return m_width; }
    void Rectangle::width(int w) { m_width = w; }
    int Rectangle::height() const { return m_height; }
    void Rectangle::height(int h) { m_height = h; }

    std::string Rectangle::to_string() const
    {
        std::string s = "geometry::Rectagle - [";
        s += std::to_string(m_x) + ", ";
        s += std::to_string(m_y) + ", ";
        s += std::to_string(m_width) + ", ";
        s += std::to_string(m_height) + "]";
        return s;
    }
}