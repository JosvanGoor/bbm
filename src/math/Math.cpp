#include "Math.hpp"

namespace math
{

    int snap_to_grid(int val, int grid)
    {
        return ((val + (grid/2)) / grid) * grid;
    }

    double stretch(double val, double min, double max, double newmin, double newmax)
    {
        //just assumes val is between min and max, else the result will just be botched.
        double range = max - min;
        double newrange = newmax - newmin;

        val -= min;
        return (((val / range) * newrange) + newmin);
    }

    /*
    int cos(int i) { return cosf(i); }
    long cos(long l) { return cosf(l); }
    float cos(float f) { return cos(f); }
    double cos(double d) { return cos(d); }

    int sin(int i) { return sinf(i); }
    long sin(long l) { return cosf(l); }
    float sin(float f) { return cosf(f); }
    double sin(double d) { return cos(d); }

    int tan(int i) { return tanf(i); }
    long tan(long l) { return tanf(l); }
    float tan(float f) { return tanf(f); }
    double tan(double d) { return tanf(d); }
    */
}
