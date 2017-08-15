#ifndef BBM_GEOMETRY_POINT_HPP
#define BBM_GEOMETRY_POINT_HPP

#include <string>
#include "../core/Stringable.hpp"

namespace geometry
{

    template<typename T> class Point : public Stringable
    {
        public:
            Point() :
                m_x(0), m_y(0) { }
            Point(T x, T y) :
                m_x(x), m_y(y) { }

            template<typename U> Point<U> as()
            {
                return Point<U>(U(m_x), U(m_y));
            }
            
            void set(T x, T y)
            {
                m_x = x;
                m_y = y;
            }

            void translate(T dx, T dy)
            {
                m_x += dx;
                m_y += dy;
            }

            float distance(const Point<T> &p)
            {
                return sqrt(pow(m_x - p.x(), 2) + pow(m_y - p.y(), 2));
            }

            T x() const { return m_x; }
            T y() const { return m_y; }

            void x(T x) { m_x = x; }
            void y(T y) { m_y = y; }

            virtual std::string to_string() const override
            {
                return "geometry::Point - [" + std::to_string(m_x) + ", " + std::to_string(m_y) + "].";
            }

        protected:
            T m_x;
            T m_y;
    };

    typedef Point<float> Pointf;
}

#endif
