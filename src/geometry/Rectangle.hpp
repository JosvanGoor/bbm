#ifndef BBM_GEOMETRY_RECTANGLE_HPP
#define BBM_GEOMETRY_RECTANGLE_HPP

#include <string>
#include "Point.hpp"
#include "../math/Math.hpp"
#include "../core/Stringable.hpp"

namespace geometry
{

    template<typename T> class Rectangle : public Stringable
    {
        public:
            Rectangle() : 
                m_x(0), m_y(0), m_width(0), m_height(0) { }
            Rectangle(T x, T y, T w, T h) :
                m_x(x), m_y(y), m_width(w), m_height(h) { }
            
            Rectangle(const Rectangle<T> &rect) : 
                m_x(rect.x()), m_y(rect.y()), m_width(rect.width()), m_height(rect.height()) { }

            template<typename U> Rectangle<U> as() const
            {
                return Rectangle<U>(U(m_x), U(m_y), U(m_width), U(m_height));
            }

            void translate(T dx, T dy)
            {
                m_x += dx;
                m_y += dy;
            }

            void set(T x, T y, T w, T h)
            {
                m_x = x;
                m_y = y;
                m_width = w;
                m_height = h;
            }

            void position(T x, T y)
            {
                m_x = x;
                m_y = y;
            }

            void snap_to_grid(int gx, int gy)
            {
                m_x = math::snap_to_grid((int)m_x, gx);
                m_y = math::snap_to_grid((int)m_y, gy);
            }

            bool contains(const Point<T> &p) const
            {
                if(p.x() < m_x || p.x() > (m_x + m_width)) return false;
                if(p.y() < m_y || p.y() > (m_y + m_height)) return false;
                return true;
            }

            bool intersects(const Rectangle<T> &r)
            {
                if(r.x() > (m_x + m_width - 1)) return false;
                if((r.x() + r.width() - 1) < m_x) return false;
        
                if(r.y() > (m_y + m_height - 1)) return false;
                if((r.y() + r.height() - 1) < m_y) return false;
        
                return true;
            }

            void x(T x) { m_x = x; }
            void y(T y) { m_y = y; }
            void width(T w) { m_width = w; }
            void height(T h) { m_height = h; }

            T x() const { return m_x; }            
            T y() const { return m_y; }
            T width() const { return m_width; }
            T height() const { return m_height; }

            virtual std::string to_string() const override
            {
                std::string s = "geometry::Rectagle - [";
                s += std::to_string(m_x) + ", ";
                s += std::to_string(m_y) + ", ";
                s += std::to_string(m_width) + ", ";
                s += std::to_string(m_height) + "]";
                return s;
            }

        protected:
            T m_x;
            T m_y;
            T m_width;
            T m_height;
    };

    typedef Rectangle<float> Rectanglef;
}

#endif