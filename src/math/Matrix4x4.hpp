#ifndef MATH_MATRIX4X4_HPP
#define MATH_MATRIX4X4_HPP

/**
    Represents a 4x4 matrix with T precision. Contains some basic math functions and can be multiplied with the Vector4 template class
    also from this namespace.

    When attempting to set or get values with x,y and using values outside 0-3 there will be no values set and always 0 returned.
    This is to keep the functions as lightweight as possible.

    The numbers use the column major layout in memory
        0:  1:  2:  3:
    0: [00, 04, 08, 12]
    1: [01, 05, 09, 13]
    2: [02, 06, 10, 14]
    3: [03, 07, 11, 15]

    The classes in the math namespace dont do error checking for safe values for the sake of performance, so you make sure no /0 happens.
    The classes in namespace math are also all castable to strings, returning readable interpretation of the object in memory.

    The standard use is float, and it is created to be used with numericals.
*/

#include <string>
#include <cstring>
#include <sstream>
#include <typeinfo>

#include "Math.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "../Core.hpp"

namespace math
{

    template<typename T> class Matrix4x4 : public Object
    {
    public:
        T m_data[16]; //array containing the numbers.

        //empty constructor, sets the identity matrix.
        Matrix4x4()
        {
            m_data[1] = m_data[2] = m_data[3] = 0;
            m_data[4] = m_data[6] = m_data[7] = 0;
            m_data[8] = m_data[9] = m_data[11] = 0;
            m_data[12] = m_data[13] = m_data[14] = 0;
            m_data[0] = m_data[5] = m_data[10] = m_data[15] = 1;
        }

        //Cosntructor, sets all values to t
        Matrix4x4(T t)
        {
            for(size_t i = 0; i < 16; i++)
            {
                m_data[i] = t;
            }
        }

        //constructor with initlist, copies the values
        Matrix4x4(T t[16])
        {
            for(size_t i = 0; i < 16; i++)
            {
                m_data[i] = t[i];
            }
        }

        //sets the x,y value to T, does nothing when x or y are invalid.
        void set(size_t x, size_t y, T t)
        {
            if(x > 3 || y > 3) return;
            m_data[(4 * x )+ y] = t;
        }

        //resets the matrix to the identity matrix
        void loadIdentity()
        {
            m_data[1] = m_data[2] = m_data[3] = 0;
            m_data[4] = m_data[6] = m_data[7] = 0;
            m_data[8] = m_data[9] = m_data[11] = 0;
            m_data[12] = m_data[13] = m_data[14] = 0;
            m_data[0] = m_data[5] = m_data[10] = m_data[15] = 1;
        }

        //rounds all values smaller then 1e-5 to 0. (used for graphical edge-case prevention)
        void clean()
        {
            for(size_t i = 0; i < 16; i++)
            {
                if(fabs(m_data[i]) < 1e-5) m_data[i] = 0;
            }
        }

        //returns a T[16] containing the values.
        const T *values() const
        {
            return values();
        }

        //returns the value at x,y. returns T(0) when x or y is invalid.
        T get(size_t x, size_t y) const
        {
            if(x > 3 || y > 3) return T(0);
            return m_data[(x * 4) + y];
        }

        //Returns the determinant of the matrix.
        T determinant() const
        {
            T s0 = m_data[0] * m_data[5] - m_data[4] * m_data[1];
            T s1 = m_data[0] * m_data[6] - m_data[4] * m_data[2];
            T s2 = m_data[0] * m_data[7] - m_data[4] * m_data[3];
            T s3 = m_data[1] * m_data[6] - m_data[5] * m_data[2];
            T s4 = m_data[1] * m_data[7] - m_data[5] * m_data[3];
            T s5 = m_data[2] * m_data[7] - m_data[6] * m_data[3];

            T c5 = m_data[10] * m_data[15] - m_data[14] * m_data[11];
            T c4 = m_data[9] * m_data[15] - m_data[13] * m_data[11];
            T c3 = m_data[9] * m_data[14] - m_data[13] * m_data[10];
            T c2 = m_data[8] * m_data[15] - m_data[12] * m_data[11];
            T c1 = m_data[8] * m_data[14] - m_data[12] * m_data[10];
            T c0 = m_data[8] * m_data[13] - m_data[12] * m_data[9];

            return (s0*c5 - s1*c4 + s2*c3 + s3*c2 - s4*c1 + s5*c0);
        }

        //returns an inversed version of this matrix
        Matrix4x4<T> inversed() const
        {
            T s0 = m_data[0] * m_data[5] - m_data[4] * m_data[1];
            T s1 = m_data[0] * m_data[6] - m_data[4] * m_data[2];
            T s2 = m_data[0] * m_data[7] - m_data[4] * m_data[3];
            T s3 = m_data[1] * m_data[6] - m_data[5] * m_data[2];
            T s4 = m_data[1] * m_data[7] - m_data[5] * m_data[3];
            T s5 = m_data[2] * m_data[7] - m_data[6] * m_data[3];

            T c5 = m_data[10] * m_data[15] - m_data[14] * m_data[11];
            T c4 = m_data[9] * m_data[15] - m_data[13] * m_data[11];
            T c3 = m_data[9] * m_data[14] - m_data[13] * m_data[10];
            T c2 = m_data[8] * m_data[15] - m_data[12] * m_data[11];
            T c1 = m_data[8] * m_data[14] - m_data[12] * m_data[10];
            T c0 = m_data[8] * m_data[13] - m_data[12] * m_data[9];

            T det = (s0*c5 - s1*c4 + s2*c3 + s3*c2 - s4*c1 + s5*c0);
            Matrix4x4<T> m;
            if(det == T(0)) return m;
            T invdet = 1.0 / det;

            m.m_data[0] = ( m_data[5]*c5 - m_data[6]*c4 + m_data[7]*c3) * invdet;
            m.m_data[1] = (-m_data[1]*c5 + m_data[2]*c4 - m_data[3]*c3) * invdet;
            m.m_data[2] = ( m_data[13]*s5 - m_data[14]*s4 + m_data[15]*s3) * invdet;
            m.m_data[3] = (-m_data[9]*s5 + m_data[10]*s4 - m_data[11]*s3) * invdet;

            m.m_data[4] = (-m_data[4]*c5 + m_data[6]*c2 - m_data[7]*c1) * invdet;
            m.m_data[5] = ( m_data[0]*c5 - m_data[2]*c2 + m_data[3]*c1) * invdet;
            m.m_data[6] = (-m_data[12]*s5 + m_data[14]*s2 - m_data[15]*s1) * invdet;
            m.m_data[7] = ( m_data[8]*s5 - m_data[10]*s2 + m_data[11]*s1) * invdet;

            m.m_data[8] = ( m_data[4]*c4 - m_data[5]*c2 + m_data[7]*c0) * invdet;
            m.m_data[9] = (-m_data[0]*c4 + m_data[1]*c2 - m_data[3]*c0) * invdet;
            m.m_data[10] = ( m_data[12]*s4 - m_data[13]*s2 + m_data[15]*s0) * invdet;
            m.m_data[11] = (-m_data[8]*s4 + m_data[9]*s2 - m_data[11]*s0) * invdet;

            m.m_data[12] = (-m_data[4]*c3 + m_data[5]*c1 - m_data[6]*c0) * invdet;
            m.m_data[13] = ( m_data[0]*c3 - m_data[1]*c1 + m_data[2]*c0) * invdet;
            m.m_data[14] = (-m_data[12]*s3 + m_data[13]*s1 - m_data[14]*s0) * invdet;
            m.m_data[15] = ( m_data[8]*s3 - m_data[9]*s1 + m_data[10]*s0) * invdet;

            return m;
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////Transformations/////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////

        //returns a version rotated t over the x axis. (uses float precision at best)
        template<typename U> Matrix4x4<T> rotate_x(U u) const
        {
            Matrix4x4<T> mult();

            mult.m_data[5] = mcos(u);
            mult.m_data[9] = msin(u);
            mult.m_data[6] = -(mult.m_data[9]);
            mult.m_data[10] = mult.m_data[5];

            return (*this) * mult;
        }

        //returns a version rotated t over the y axis. (uses float precision at best)
        template<typename U> Matrix4x4<T> rotate_y(U u) const
        {
            Matrix4x4<T> mult;

            mult.m_data[0] = mcos(u);
            mult.m_data[8] = -msin(u);
            mult.m_data[2] = -(mult.m_data[8]);
            mult.m_data[10] = mult.m_data[0];

            return (*this) * mult;
        }

        //returns a version rotated t over the z axis. (uses float precision at best)
        template<typename U> Matrix4x4<T> rotate_z(U u) const
        {
            Matrix4x4<T> mult;

            mult.m_data[0] = mcos(u);
            mult.m_data[4] = msin(u);
            mult.m_data[1] = -(mult.m_data[4]);
            mult.m_data[5] = mult.m_data[0];

            return (*this) * mult;
        }

        //returns a scaled version
        template<typename U> Matrix4x4<T> scale(U x, U y, U z) const
        {
            Matrix4x4<T> mult;

            mult.m_data[0] = x;
            mult.m_data[5] = y;
            mult.m_data[10] = z;

            return (*this) * mult;
        }

        //returns a translated version
        template<typename U> Matrix4x4<T> translate(U x, U y, U z) const
        {
            Matrix4x4<T> mult;

            mult.m_data[3] = x;
            mult.m_data[7] = y;
            mult.m_data[11] = z;

            return (*this) * mult;
        }

        ///////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////Operators////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////

        template<typename U> bool operator==(const Matrix4x4<U> &m) const
        {
            for(size_t i = 0; i < 16; i++)
                if(m_data[i] != m.m_data[i]) return false;

            return false;
        }

        template<typename U> Matrix4x4<T> operator+(const Matrix4x4<U> &m) const
        {
            Matrix4x4<T> rval;

            for(size_t i = 0; i < 16; i++)
                rval.m_data[i] = m_data[i] + m.m_data[i];

            return rval;
        }

        template<typename U> Matrix4x4<T> operator-(const Matrix4x4<U> &m) const
        {
            Matrix4x4<T> rval;

            for(size_t i = 0; i < 16; i++)
                rval.m_data[i] = m_data[i] - m.m_data[i];

            return rval;
        }

        template<typename U> Matrix4x4<T> operator*(const Matrix4x4<U> &m) const
        {
            Matrix4x4<T> rval(.0f);

            for(size_t i = 0; i < 4; i++)
            {
                for(size_t j = 0; j < 4; j++)
                {
                    for(size_t x = 0; x < 4; x++)
                    {
                        rval.m_data[(4*i)+j] = rval.m_data[(4*i)+j] + m_data[(4*x)+j] * m.m_data[(4*i)+x];
                    }
                }
            }
            return rval;
        }

        template<typename U> Vector4<T> operator*(const Vector4<U> &v) const
        {
            Vector4<T> rval;

            rval.x = v.x * m_data[0] + v.y * m_data[4] + v.y * m_data[8] + v.z * m_data[12];
            rval.y = v.x * m_data[1] + v.y * m_data[5] + v.y * m_data[9] + v.z * m_data[13];
            rval.z = v.x * m_data[2] + v.y * m_data[6] + v.y * m_data[10] + v.z * m_data[14];
            rval.w = v.x * m_data[3] + v.y * m_data[7] + v.y * m_data[11] + v.z * m_data[15];

            return rval;
        }

        virtual std::string toString() const
        {
            std::stringstream ss;
            //TODO: find out why typeinfo throws a segfault
            ss << "ez::Math::Matrix4<" /*<< typeid(T).name()*/ << ">:" << "\n";
            ss << "    [" << m_data[0] << ", " << m_data[4] << ", " << m_data[8] << ", " << m_data[12] << "]\n";
            ss << "    [" << m_data[1] << ", " << m_data[5] << ", " << m_data[9] << ", " << m_data[13] << "]\n";
            ss << "    [" << m_data[2] << ", " << m_data[6] << ", " << m_data[10] << ", " << m_data[14] << "]\n";
            ss << "    [" << m_data[3] << ", " << m_data[7] << ", " << m_data[11] << ", " << m_data[15] << "]";
            return ss.str();
        }

    };

    //generates a lookat matrix (used in calculating MVP matrix)
    template<typename T> Matrix4x4<T> lookAt(Vector3<T> eye, Vector3<T> center, Vector3<T> up)
    {
        Vector3<T> f = (center - eye).normalized();
        Vector3<T> u = up.normalized();
        Vector3<T> s = f.cross(u).normalized();
        u = s.cross(f);

        Matrix4x4<T> rval;
        rval.m_data[0] = s.x;
        rval.m_data[4] = s.y;
        rval.m_data[8] = s.z;

        rval.m_data[1] = u.x;
        rval.m_data[5] = u.y;
        rval.m_data[9] = u.z;

        rval.m_data[2] = -f.x;
        rval.m_data[6] = -f.y;
        rval.m_data[10] = -f.z;

        rval.m_data[3] = -(s.dot(eye));
        rval.m_data[7] = -(u.dot(eye));
        rval.m_data[11] = f.dot(eye);

        return rval;
    }

    //generates an orthographic projection matrix (used for calculating 2d MVP matrix)
    template<typename T> Matrix4x4<T> orthographicProjection(T left, T right, T bottom, T top, T znear, T zfar)
    {
        Matrix4x4<T> rval;

        rval.m_data[0] = T(2.0f) / (right - left);
        rval.m_data[5] = T(2.0f) / (top - bottom);
        rval.m_data[10] = T(2.0f) / (zfar - znear);
        rval.m_data[12] = -((right + left) / (right - left));
        rval.m_data[13] = -((top + bottom) / (top - bottom));
        rval.m_data[14] = -((zfar + znear) / (zfar - znear));

        return rval;

    }

    //generates a perspective projection matrix (used for calculating 3d MVP matrix)
    template<typename T> Matrix4x4<T> perspectiveProjection(T fov, T aspect, T znear, T zfar)
    {
        T rad = dtor(fov);

        T range = tanf(rad / T(2.0f)) * znear;
        T left = -range * aspect;
        T right = range * aspect;
        T bottom = -range;
        T top = range;

        Matrix4x4<T> rval(0);
        rval.m_data[0] = (T(2.0f) * znear) / (right - left);
        rval.m_data[5] = (T(2.0f) * znear) / (top - bottom);
        rval.m_data[10] = -(zfar + znear) / (zfar - znear);
        rval.m_data[11] = T(-1.0f);
        rval.m_data[14] = -(T(2.0f) * zfar * znear) / (zfar - znear);

        return rval;
    }

}

#endif // MATH_MATRIX4_H
