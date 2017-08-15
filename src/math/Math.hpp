#ifndef EZ_MATH_MATH_HPP
#define EZ_MATH_MATH_HPP
/*
    This file contains some basic functions usable for math
*/

#include <cmath>

namespace math
{

    //pi defined as relevant as needed
    static double pi = 3.14159265359; //source: google calculator

    //Returns the square of T
    template<typename T> T pow2(T t)
    {
        return t * t;
    }

    //converts T in radians to degrees.
    template<typename T> T rtod(T t)
    {
        return t * T(180.0 / pi);
    }

    //converts T in degrees to radians
    template<typename T> T dtor(T t)
    {
        return t * (pi / 180.0);
    }

    //snaps t to gridsize gs (max value is intmax).
    template<typename T> T snapToGrid(T t, T gs)
    {
        t + T(0.5 * gs);
        t = t / gs;
        return ((int)t) * gs;
    }

    int snap_to_grid(int val, int grid);
    double stretch(double val, double min, double max, double newmin = 0.0, double newmax = 1.0);
}
#endif // MATH_MATH_H
