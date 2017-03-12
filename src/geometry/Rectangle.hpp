#ifndef BBM_GEOMETRY_RECTANGLE_HPP
#define BBM_GEOMETRY_RECTANGLE_HPP

#include <string>
#include "Point.hpp"
#include "../Core.hpp"

namespace geometry
{

    /*
        2d rectangle, axis-aligned.
    */

    class Rectangle : public Object
    {
    public:
        Rectangle(int x = 0, int y = 0, int w = 0, int h = 0);

        void translate(int dx, int dy);
        void set(int x, int y, int w, int h);

        bool contains(const Point &p);
        bool intersects(const Rectangle &r);
        
        int x() const;
        void x(int x);
        int y() const;
        void y(int y);
        int width() const;
        void width(int w);
        int height() const;
        void height(int height);

        virtual std::string to_string() const;

    protected:
        int m_x;
        int m_y;
        int m_width;
        int m_height;
    };

}

#endif