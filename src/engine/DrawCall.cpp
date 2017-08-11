#include "DrawCall.hpp"
#include "../bomberman/Bomberman.hpp"

#include <iostream>

namespace engine
{
    
    DrawCall::DrawCall(const std::vector<GLfloat> &data, const std::string &tex)
    {
        glGenVertexArrays(1, &m_vertex_array);
        glBindVertexArray(m_vertex_array);

        glGenBuffers(1, &m_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        int stride = (3 + 2) * sizeof(GLfloat);
        void* voffset = (void*)0;
        void *toffset = (void*)(3 * sizeof(GLfloat));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, voffset);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, toffset);

        m_texture = tex;
        m_triangles_count = data.size() / 5;

        m_color = math::Vector4f(1.0, 1.0, 1.0, 1.0);
        m_transform = math::Matrix4f();
    }

    DrawCall::~DrawCall()
    {
        glDeleteVertexArrays(1, &m_vertex_array);
        glDeleteBuffers(1, &m_vertex_buffer);
    }

    void DrawCall::draw() const
    {
        glBindVertexArray(m_vertex_array);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Bomberman::instance().texture_cache().get_texture(m_texture));

        glUniform4f(Bomberman::instance().shloc_color_filter(), m_color.x(), m_color.y(), m_color.z(), m_color.w());
        glUniformMatrix4fv(Bomberman::instance().shloc_translation(), 1, GL_FALSE, m_transform.data());
        glUniformMatrix4fv(Bomberman::instance().shloc_view(), 1, GL_FALSE, Bomberman::instance().default_view().data());
        glUniformMatrix4fv(Bomberman::instance().shloc_projection(), 1, GL_FALSE, Bomberman::instance().projection().data());
        
        glDrawArrays(GL_TRIANGLES, 0, m_triangles_count);

        glBindVertexArray(0);
    }

    void DrawCall::draw(const math::Matrix4f &transform, const std::string &texture) const
    {
        glBindVertexArray(m_vertex_array);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Bomberman::instance().texture_cache().get_texture(texture));

        glUniform4f(Bomberman::instance().shloc_color_filter(), m_color.x(), m_color.y(), m_color.z(), m_color.w());
        glUniformMatrix4fv(Bomberman::instance().shloc_translation(), 1, GL_FALSE, transform.data());
        glUniformMatrix4fv(Bomberman::instance().shloc_view(), 1, GL_FALSE, Bomberman::instance().default_view().data());
        glUniformMatrix4fv(Bomberman::instance().shloc_projection(), 1, GL_FALSE, Bomberman::instance().projection().data());
        
        glDrawArrays(GL_TRIANGLES, 0, m_triangles_count);

        glBindVertexArray(0);
    }

    void DrawCall::draw(const geometry::Rectanglef &rect, const std::string &texture) const
    {
        math::Matrix4f tf;
        tf = tf.scale(rect.width(), rect.height(), 0.0f);
        tf = tf.translate(rect.x(), rect.y(), 0.0f);
        draw(tf, texture);
    }

    math::Matrix4f DrawCall::transform() const { return m_transform; }
    void DrawCall::transform(const math::Matrix4f &transform) { m_transform = transform; }
    math::Vector4f DrawCall::color() const { return m_color; }
    void DrawCall::color(const math::Vector4f &color) { m_color = color; }
    std::string DrawCall::texture() const { return m_texture; }
    void DrawCall::texture(const std::string &tex) { m_texture = tex; }

}