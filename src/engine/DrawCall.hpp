#ifndef ENGINE_DRAWCALL_HPP
#define ENGINE_DRAWCALL_HPP

#include <string>
#include <vector>
#include "../geometry/Rectangle.hpp"
#include "../math/Vector4.hpp"
#include "../math/Matrix4x4.hpp"
#include "../extern/gl_core_4_4.h"

namespace engine
{

    /*
        This class represents a standardized/simple opengl drawcall.
        This class expects data of the format: vvvttvvvttvvvtt for each triangle.
    */

    class DrawCall 
    {
        public:
            DrawCall(const std::vector<GLfloat> &data, const std::string &texture);
            ~DrawCall();

            //delete copy etc.
            DrawCall(const DrawCall&) = delete;
            DrawCall(DrawCall&&) = delete;
            void operator=(const DrawCall&) = delete;
            void operator=(DrawCall&&) = delete;

            virtual void draw() const;
            //for common shapes but rapidly changing textures etc.
            virtual void draw(const math::Matrix4f &transform, const std::string &tex) const;
            virtual void draw(const geometry::Rectanglef &rect, const std::string &tex) const;

            math::Matrix4f transform() const;
            void transform(const math::Matrix4f &transform);
            math::Vector4f color() const;
            void color(const math::Vector4f &color);
            std::string texture() const;
            void texture(const std::string &tex);

        protected:
            DrawCall() {};

            std::string m_texture;
            math::Vector4f m_color;
            math::Matrix4f m_transform;
            
            GLuint m_vertex_array;
            GLuint m_vertex_buffer;
            size_t m_triangles_count;
    };

    //std::vector<GLfloat> interleave(const std::vector<GLfloat> &v, const std::vector<GLfloat> &t);

}

#endif