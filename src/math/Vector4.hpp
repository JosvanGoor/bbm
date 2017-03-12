#ifndef MATH_VECTOR4_H
#define MATH_VECTOR4_H

/**
    This class represents a 4d vector, can be used with any numerical type or class that has its mathmatical operators in place.
    The classes in the math namespace dont do error checking for safe values for the sake of performance, so you make sure no /0 happens.
    The classes in namespace math are also all castable to strings, returning readable interpretation of the object in memory.

    The standard use is float, and it is created to be used with numericals.
 */

#include <cmath>
#include <sstream>
#include <typeinfo>

#include "Math.hpp"
#include "../Core.hpp"

namespace math
{

    template<typename T = float> class Vector4 : public Object
    {
    public:
        T m_x;
        T m_y;
        T m_z;
        T m_w;

        //clean, per-value and copy constructor
        Vector4() : m_x(), m_y(), m_z(), m_w() {}
        Vector4(T x, T y, T z, T w) : m_x(x), m_y(y), m_z(z), m_w(w) {}
        template<typename U> Vector4(const Vector4<U> &v) : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z), m_w(v.m_w) {}

        //returns a const T[4] with the values.
        const T *values() const
        {
            return (T*)this;
        }

        //normalizes this vector
        //!! Dont call this when the length is 0.
        void normalize()
        {
            T l = length();

            m_x = m_x / l;
            m_y = m_y / l;
            m_z = m_z / l;
            m_w = m_w / l;
        }

        //returns a normalized version of this vector.
        Vector4<T> normalized() const
        {
            Vector4<T> rval;
            T l = length();

            rval.m_x = m_x / l;
            rval.m_y = m_y / l;
            rval.m_z = m_z / l;
            rval.m_w = m_w / l;

            return rval;
        }

        //returns the length of this vector
        T length() const
        {
            return sqrt(length2());
        }

        //returns the squared length of this vector
        T length2() const
        {
            return pow2(m_x) + pow2(m_y) + pow2(m_z) + pow2(m_w);
        }

        T dot(const Vector4<T> &v) const
        {
            return (m_x*v.m_x) + (m_y*v.m_y) + (m_z*v.m_z) + (m_w*v.m_w);
        }

        void clamp(T min, T max)
        {
            m_x = m_x < min ? min : (m_x > max ? max : m_x);
            m_y = m_y < min ? min : (m_y > max ? max : m_y);
            m_z = m_z < min ? min : (m_z > max ? max : m_z);
            m_w = m_w < min ? min : (m_w > max ? max : m_w);
        }

        Vector4<T> clamp(T min, T max) const
        {
            Vector4<T> rval;

            rval.m_x = m_x < min ? min : (m_x > max ? max : m_x);
            rval.m_y = m_y < min ? min : (m_y > max ? max : m_y);
            rval.m_z = m_z < min ? min : (m_z > max ? max : m_z);
            rval.m_w = m_w < min ? min : (m_w > max ? max : m_w);

            return rval;
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////Operators////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////

        Vector4<T> operator-() const
        {
            return Vector4<T>(-m_x, -m_y, -m_z, -m_w);
        }

        template<typename U> Vector4<T> &operator=(const Vector4<U> &v)
        {
            m_x = v.m_x;
            m_y = v.m_y;
            m_z = v.m_z;
            m_w = v.m_w;

            return *this;
        }

        template<typename U> Vector4<T> operator+(const Vector4<U> &v) const
        {
            return Vector4<T>(m_x+v.m_x, m_y+v.m_y, m_z+v.m_z, m_w+v.m_w);
        }

        template<typename U> Vector4<T> operator-(const Vector4<U> &v) const
        {
            return Vector4<T>(m_x-v.m_x, m_y-v.m_y, m_z-v.m_z, m_w-v.m_w);
        }

        //vector * vector is the dot product, and vector / vector does not exist.

        template<typename U> Vector4<T> &operator+=(const Vector4<U> &v)
        {
            m_x = m_x + v.m_x;
            m_y = m_y + v.m_y;
            m_z = m_z + v.m_z;
            m_w = m_w + v.m_w;

            return *this;
        }

        template<typename U> Vector4<T> &operator-=(const Vector4<U> &v)
        {
            m_x = m_x - v.m_x;
            m_y = m_y - v.m_y;
            m_z = m_z - v.m_z;
            m_w = m_w - v.m_w;

            return *this;
        }

        template<typename U> bool operator==(const Vector4<U> &v) const
        {
            return (m_x == v.m_x) && (m_y == v.m_y) && (m_z == v.m_z) && (m_w == v.m_w);
        }

        template<typename U> bool operator!=(const Vector4<U> &v) const
        {
            return (m_x != v.m_x) || (m_y != v.m_y) || (m_z != v.m_z) || (m_w != v.m_w);
        }

        template<typename U> Vector4<T> operator+(U u) const
        {
            return Vector4<T>(m_x+u, m_y+u, m_z+u, m_w+u);
        }

        template<typename U> Vector4<T> operator-(U u) const
        {
            return Vector4<T>(m_x-u, m_y-u, m_z-u, m_w-u);
        }

        template<typename U> Vector4<T> operator*(U u) const
        {
            return Vector4<T>(m_x*u, m_y*u, m_z*u, m_w*u);
        }

        template<typename U> Vector4<T> operator/(U u) const
        {
            return Vector4<T>(m_x/u, m_y/u, m_z/u, m_w/u);
        }

        template<typename U> Vector4<T> &operator+=(U u)
        {
            m_x = m_x + u;
            m_y = m_y + u;
            m_z = m_z + u;
            m_w = m_w + u;

            return *this;
        }

        template<typename U> Vector4<T> &operator-=(U u)
        {
            m_x = m_x - u;
            m_y = m_y - u;
            m_z = m_z - u;
            m_w = m_w - u;

            return *this;
        }

        template<typename U> Vector4<T> &operator*=(U u)
        {
            m_x = m_x * u;
            m_y = m_y * u;
            m_z = m_z * u;
            m_w = m_w * u;

            return *this;
        }

        template<typename U> Vector4<T> &operator/=(U u)
        {
            m_x = m_x / u;
            m_y = m_y / u;
            m_z = m_z / u;
            m_w = m_w / u;

            return *this;
        }

        virtual std::string toString() const
        {
            std::stringstream ss;
            ss << "ez::Math::Vector4<" /*<< typeid(T).name()*/ << ">: [" << m_x << ", " << m_y << ", " << m_z << ", " << m_w << "].";
            return ss.str();
        }
    };

}

#endif // MATH_VECTOR4_H
