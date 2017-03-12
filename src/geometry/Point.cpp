#include "Point.hpp"

#include <cmath>

namespace geometry
{

    Point::Point(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

    void Point::set(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

    void Point::translate(int dx, int dy)
    {
        m_x += dx;
        m_y += dy;
    }

    float Point::distance(const Point &p)
    {
        return sqrt(pow(m_x - p.m_x, 2) + pow(m_y - p.m_y, 2));
    }

    int Point::x() const { return m_x; }
    void Point::x(int x) { m_x = x; }
    int Point::y() const { return m_y; }
    void Point::y(int y) { m_y = y; }

    std::string Point::to_string() const
    {
        return "geometry::Point - [" + std::to_string(m_x) + ", " + std::to_string(m_y) + "].";
    }

}