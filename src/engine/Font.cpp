#include "Font.hpp"

#include <fstream>
#include "../utility/Utility.hpp"
#include "../bomberman/Bomberman.hpp"

namespace engine
{

    Font::Font(const std::string &font)
    {
        if(!Bomberman::instance().texture_cache().has_texture(font + ".png"))
        {
            Bomberman::instance().texture_cache().load_nonscaling_texture(font + ".png");
        }
        parse_info(font + ".info");
        m_texture = font + ".png";
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

    bool Font::is_valid(const std::string &str) const
    {
        for(auto c : str) if (!contains(c)) return false;
        return true;
    }

    RenderableString* Font::renderable_string(const std::string &str)
    {
        std::cout << __PRETTY_FUNCTION__ << " called with argument " << str << std::endl;

        std::vector<GLfloat> data;
        data.reserve(str.length() * 2 * 5 * 3);

        size_t triangles;
        float advance = 0;
        float ypos = 0;
        float tex_width = 1.0 / m_sheet_width;
        float tex_height = 1.0 / m_sheet_height;

        float spr_width = (float)m_sprite_width;
        float spr_height = (float)m_sprite_height;

        for(char c : str)
        {
            if(c == ' ')
            {
                advance += m_advance * 3;
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

            data.insert(data.end(), {advance, ypos, 0,                          tex_x, tex_y});
            data.insert(data.end(), {advance + spr_width, ypos, 0,              tex_x + tex_width, tex_y});
            data.insert(data.end(), {advance, ypos + spr_height, 0,             tex_x, tex_y + tex_height});
            data.insert(data.end(), {advance + spr_width, ypos, 0,              tex_x + tex_width, tex_y});
            data.insert(data.end(), {advance + spr_width, ypos + spr_height, 0, tex_x + tex_width, tex_y + tex_height});
            data.insert(data.end(), {advance, ypos + spr_height, 0,             tex_x, tex_y + tex_height});

            advance += m_advance + widths[cindex];
            triangles += 2;
        }

        std::cout << "Font generated " << data.size() << " floats" << std::endl;
        RenderableString *rs = new RenderableString(data, m_texture);
        rs->m_height = m_sprite_height;
        rs->m_width = advance;
        rs->m_string = str;
        
        return rs;
    }

    void Font::renderable_string(RenderableString *rs, const std::string &str)
    {
        std::vector<GLfloat> floats;
        floats.reserve(str.length() * 2 * 5 * 3);

        size_t triangles = 0;
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
            triangles += 2;
        }

        glBindBuffer(GL_ARRAY_BUFFER, rs->m_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * floats.size(), floats.data(), GL_STATIC_DRAW);

        rs->m_height = m_sprite_height;
        rs->m_width = advance;
        rs->m_texture = m_texture;
        rs->m_string = str;
        rs->m_triangles_count = floats.size() / 5;
    }

    void Font::parse_info(const std::string &file)
    {
        std::ifstream in(file);
        if(in.fail())
            throw Exception(__PRETTY_FUNCTION__, "Failed to find info file for " + file);

        //POOR ERRORCHECKING
        m_sheet_width = std::stoi(utility::get_noncomment_line(in));
        m_sheet_height = std::stoi(utility::get_noncomment_line(in));
        m_sprite_width = std::stoi(utility::get_noncomment_line(in));
        m_sprite_height = std::stoi(utility::get_noncomment_line(in));
        m_first = std::stoi(utility::get_noncomment_line(in));
        m_advance = std::stoi(utility::get_noncomment_line(in));
        m_newline_advance = std::stoi(utility::get_noncomment_line(in));

        widths = new int[m_sheet_width * m_sheet_height];
        for(size_t i = 0; i < (m_sheet_width * m_sheet_height); ++i)
        {
            widths[i] = std::stoi(utility::get_noncomment_line(in));
        }
    }

}