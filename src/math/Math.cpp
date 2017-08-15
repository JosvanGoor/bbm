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
    
}
