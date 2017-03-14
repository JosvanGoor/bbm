#ifndef BBM_GEOMETRY_POINT_HPP
#define BBM_GEOMETRY_POINT_HPP

#include <string>
#include "../Core.hpp"

namespace geometry
{

    /*
        2d point
    */

    class Point : public Object
    {
        public:
            Point(int x = 0, int y = 0);

            void set(int x, int y);
            void translate(int dx, int dy);
            float distance(const Point &p);

            int x() const;
            void x(int x);
            int y() const;
            void y(int y);

            virtual std::string to_string() const;
        protected:
            int m_x;
            int m_y;
    };

}

#endif
