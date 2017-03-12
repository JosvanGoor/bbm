#ifndef MATH_VECTOR3_H
#define MATH_VECTOR3_H

/**
    This class represents a 3d vector, can be used with any numerical type or class that has its mathmatical operators in place.
    The classes in the math namespace dont do errorchecking for safe values for the sake of performance, so you make sure no /0 happens.
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

    template<typename T = float> class Vector3 : public Object
    {
    public:
        T m_x;
        T m_y;
        T m_z;

        //Clean, per-value and copy constructor.
        Vector3() : m_x(), m_y(), m_z() {}
        Vector3(T x, T y, T z) : m_x(x), m_y(y), m_z(z) {}
        template<typename U> Vector3(const Vector3<U> &v) : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z) {}

        //returns a T[3] containing the values in x,y,z order.
        const T *values() const
        {
            return (T*)this;
        }

        //Normalizes this vector (makes its length 1)
        //!! Dont call this when the length is 0.
        void normalize()
        {
            T l = length();
            m_x = m_x / l;
            m_y = m_y / l;
            m_z = m_z / l;
        }

        //returns a normalized version of this vector
        Vector3<T> normalized() const
        {
            Vector3<T> rval;
            T l = length();

            rval.m_x = m_x / l;
            rval.m_y = m_y / l;
            rval.m_z = m_z / l;

            return rval;
        }

        //Returns the length of this vector.
        T length() const
        {
            return sqrt(pow2(m_x) + pow2(m_y) + pow2(m_z));
        }

        //calculates the dot product between this and the passed vector
        template<typename U> T dot(const Vector3<U> &v) const
        {
            return (m_x * v.m_x) + (m_y * v.m_y) + (m_z * v.m_z);
        }

        //returns the cross product between this and the passed vector
        template<typename U> Vector3<T> cross(const Vector3<U> &v) const
        {
            Vector3<T> rval;

            rval.m_x = (m_y * v.m_z) - (m_z * v.m_y);
            rval.m_y = (m_z * v.m_x) - (m_x * v.m_z);
            rval.m_z = (m_x * v.m_y) - (m_y * v.m_x);

            return rval;
        }

        //clamps this vector
        void clamp(T min, T max)
        {
            m_x = m_x < min ? min : (m_x > max ? max : m_x);
            m_y = m_y < min ? min : (m_y > max ? max : m_y);
            m_z = m_z < min ? min : (m_z > max ? max : m_z);
        }

        //returns a clamped version of this vector
        Vector3<T> clamped(T min, T max) const
        {
            Vector3<T> rval;

            rval.m_x = m_x < min ? min : (m_x > max ? max : m_x);
            rval.m_y = m_y < min ? min : (m_y > max ? max : m_y);
            rval.m_z = m_z < min ? min : (m_z > max ? max : m_z);

            return rval;
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////Operators////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////

        //returns this vector * -1.
        Vector3<T> operator-() const
        {
            return Vector3<T>(-m_x, -m_y, -m_z);
        }

        //operators vector vs vector
        template<typename U> Vector3<T> &operator=(const Vector3<U> &v)
        {
            m_x = v.m_x;
            m_y = v.m_y;
            m_z = v.m_z;

            return *this;
        }

        template<typename U> Vector3<T> operator+(const Vector3<U> &v) const
        {
            return Vector3<T>(m_x+v.m_x, m_y+v.m_y, m_z+v.m_z);
        }

        template<typename U> Vector3<T> operator-(const Vector3<U> &v) const
        {
            return Vector3<T>(m_x-v.m_x, m_y-v.m_y, m_z-v.m_z);
        }

        //Vector multiplication is the dot product and division does not exist.

        template<typename U> Vector3<T> &operator+=(const Vector3<U> &v)
        {
            m_x = m_x + v.m_x;
            m_y = m_y + v.m_y;
            m_z = m_z + v.m_z;

            return *this;
        }

        template<typename U> Vector3<T> &operator-=(const Vector3<U> &v)
        {
            m_x = m_x - v.m_x;
            m_y = m_y - v.m_y;
            m_z = m_z - v.m_z;

            return *this;
        }

        template<typename U> bool operator==(const Vector3<U> &v)
        {
            return (m_x == v.m_x) && (m_y == v.m_y) && (m_z == v.m_z);
        }

        template<typename U> bool operator!=(const Vector3<U> &v)
        {
            return (m_x != v.m_x) || (m_y != v.m_y) || (m_z != v.m_z);
        }

        //operators vector vs numerical
        template<typename U> Vector3<T> operator+(U u) const
        {
            return Vector3<T>(m_x + u, m_y + u, m_z + u);
        }

        template<typename U> Vector3<T> operator-(U u) const
        {
            return Vector3<T>(m_x - u, m_y - u, m_z - u);
        }

        template<typename U> Vector3<T> operator*(U u) const
        {
            return Vector3<T>(m_x * u, m_y * u, m_z * u);
        }

        template<typename U> Vector3<T> operator/(U u) const
        {
            return Vector3<T>(m_x / u, m_y / u, m_z / u);
        }

        template<typename U> Vector3<T> &operator+=(U u)
        {
            m_x = m_x + u;
            m_y = m_y + u;
            m_z = m_z + u;
            return *this;
        }

        template<typename U> Vector3<T> &operator-=(U u)
        {
            m_x = m_x - u;
            m_y = m_y - u;
            m_z = m_z - u;
            return *this;
        }

        template<typename U> Vector3<T> &operator*=(U u)
        {
            m_x = m_x * u;
            m_y = m_y * u;
            m_z = m_z * u;
            return *this;
        }

        template<typename U> Vector3<T> &operator/=(U u)
        {
            m_x = m_x / u;
            m_y = m_y / u;
            m_z = m_z / u;
            return *this;
        }

        //toString funcion
        virtual std::string toString() const
        {
            std::stringstream ss;
            //TODO: Why is typeid throwing segfault?
            ss << "ez::Math::Vector3<" /*<< typeid(T).name()*/ << ">: [" << m_x << ", " << m_y << ", " << m_z << "].";
            return ss.str();
        }
    };

}

#endif // WAVY_MATH_H
