#include "Font.hpp"

#include <fstream>
#include "../Core.hpp"
#include "Bomberman.hpp"

namespace engine
{

    Font::Font(const std::string &font)
    {
        if(Bomberman::instance().texture_cache().has_texture(font + ".png"))
            m_texture = Bomberman::instance().texture_cache().get_texture(font + ".png");
        else
        {
            m_texture = Bomberman::instance().texture_cache().load_nonscaling_texture(font + ".png");
        }

        parse_info(font + ".info");
    }

    Font::~Font()
    {   
        if(widths) delete[] widths;
        widths = nullptr;
    }

    bool Font::contains(char c) const 
    {
        return true;
        if(c == '\n' || c == ' ') return true;
        if(c < m_first) return false;
        if(c >= (char)(m_first + (m_sheet_width * m_sheet_height))) return false;
        return true;
    }

    bool Font::valid(const std::string &str)
    {
        for(auto c : str) if(!contains(c)) return false;
        return true;
    }

    RenderableString* Font::renderable_string(const std::string &str)
    {
        std::vector<GLfloat> floats;
        floats.reserve(str.length() * 2 * 5 * 3);

        float advance = 0;
        float ypos = 0;
        float tex_width = 1.0 / m_sheet_width;
        float tex_height = 1.0 / m_sheet_height;

        float spr_width = (float)(m_sprite_width);
        float spr_height = (float)(m_sprite_height);

        for(char c : str)
        {
            if(c == ' ')
            {
                advance += m_advance*3;
                continue;
            }
            else if(c == '\n')
            {
                ypos += m_sprite_height;
                advance = 0;
                continue;
            }
            else if(!contains(c)) continue;

            int cindex = c - m_first;
            float tex_x = cindex % m_sheet_width * tex_width;
            float tex_y = cindex / m_sheet_width * tex_height;

            floats.insert(floats.end(), {advance, ypos, 0,                     tex_x, tex_y});
            floats.insert(floats.end(), {advance + spr_width, ypos, 0,         tex_x + tex_width, tex_y});
            floats.insert(floats.end(), {advance, ypos + spr_height, 0,            tex_x, tex_y + tex_height});
            floats.insert(floats.end(), {advance + spr_width, ypos, 0,         tex_x + tex_width, tex_y});
            floats.insert(floats.end(), {advance + spr_width, ypos + spr_height, 0,tex_x + tex_width, tex_y + tex_height});
            floats.insert(floats.end(), {advance, ypos + spr_height, 0,            tex_x, tex_y + tex_height});

            advance += m_advance + widths[cindex];
        }

        RenderableString *rs = new RenderableString();
        rs->m_height = m_sprite_height;
        rs->m_width = advance;
        rs->m_texture = m_texture;
        rs->m_string = str;
        rs->m_color = math::Vector3<float>(1.0, 1.0, 1.0);
        rs->m_transform = math::Matrix4x4<float>();


        glGenVertexArrays(1, &(rs->m_vertex_array));
        glBindVertexArray(rs->m_vertex_array);

        glGenBuffers(1, &(rs->m_vertex_buffer));
        glBindBuffer(GL_ARRAY_BUFFER, rs->m_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * floats.size(), floats.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        int stride = (3 + 2) * sizeof(GLfloat);
        void *voffset = (void*)0;
        void *toffset = (void*)(3 * sizeof(GLfloat));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, voffset);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, toffset);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return rs;
    }

    void Font::renderable_string(RenderableString *rs, const std::string &str)
    {
        std::vector<GLfloat> floats;
        floats.reserve(str.length() * 2 * 5 * 3);

        float advance = 0;
        float ypos = 0;
        float tex_width = 1.0 / m_sheet_width;
        float tex_height = 1.0 / m_sheet_height;

        float spr_width = (float)(m_sprite_width);
        float spr_height = (float)(m_sprite_height);

        for(char c : str)
        {
            if(c == ' ')
            {
                advance += m_advance*3;
                continue;
            }
            else if(c == '\n')
            {
                ypos += m_sprite_height;
                advance = 0;
                continue;
            }
            else if(!contains(c)) continue;

            int cindex = c - m_first;
            float tex_x = cindex % m_sheet_width * tex_width;
            float tex_y = cindex / m_sheet_width * tex_height;

            floats.insert(floats.end(), {advance, ypos, 0,                     tex_x, tex_y});
            floats.insert(floats.end(), {advance + spr_width, ypos, 0,         tex_x + tex_width, tex_y});
            floats.insert(floats.end(), {advance, ypos + spr_height, 0,            tex_x, tex_y + tex_height});
            floats.insert(floats.end(), {advance + spr_width, ypos, 0,         tex_x + tex_width, tex_y});
            floats.insert(floats.end(), {advance + spr_width, ypos + spr_height, 0,tex_x + tex_width, tex_y + tex_height});
            floats.insert(floats.end(), {advance, ypos + spr_height, 0,            tex_x, tex_y + tex_height});

            advance += m_advance + widths[cindex];
        }

        glBindBuffer(GL_ARRAY_BUFFER, rs->m_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * floats.size(), floats.data(), GL_STATIC_DRAW);

        rs->m_height = m_sprite_height;
        rs->m_width = advance;
        rs->m_texture = m_texture;
        rs->m_string = str;
    }

    void Font::parse_info(const std::string &file)
    {
        std::ifstream in(file);
        if(in.fail())
            throw Exception(__PRETTY_FUNCTION__, "Failed to find info file for " + file);

        //POOR ERRORCHECKING
        m_sheet_width = std::stoi(get_noncomment_line(in));
        m_sheet_height = std::stoi(get_noncomment_line(in));
        m_sprite_width = std::stoi(get_noncomment_line(in));
        m_sprite_height = std::stoi(get_noncomment_line(in));
        m_first = std::stoi(get_noncomment_line(in));
        m_advance = std::stoi(get_noncomment_line(in));
        m_newline_advance = std::stoi(get_noncomment_line(in));

        widths = new int[m_sheet_width * m_sheet_height];
        for(size_t i = 0; i < (m_sheet_width * m_sheet_height); ++i)
        {
            widths[i] = std::stoi(get_noncomment_line(in));
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    // Renderable string
    //////////////////////////////////////////////////////////////////////////////

    RenderableString::~RenderableString()
    {
        glDeleteBuffers(1, &m_vertex_buffer);
    }

    void RenderableString::draw() const
    {
        glBindVertexArray(m_vertex_array);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        glUniform3f(Bomberman::instance().shloc_color_filter(), m_color.x(), m_color.y(), m_color.z());
        glUniformMatrix4fv(Bomberman::instance().shloc_translation(), 1, GL_FALSE, m_transform.data());
        glUniformMatrix4fv(Bomberman::instance().shloc_projection(), 1, GL_FALSE, Bomberman::instance().projection().data());
        
        glDrawArrays(GL_TRIANGLES, 0, 2 * 3 * m_string.size());

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    //spul

    void RenderableString::transform(const math::Matrix4x4<float> &tf)
    {
        m_transform = tf;
    }
}