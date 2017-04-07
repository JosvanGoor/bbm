#ifndef ENGINE_FONT_HPP
#define ENGINE_FONT_HPP

#include <string>
#include "../math/Vector3.hpp"
#include "../math/Matrix4x4.hpp"
#include "../extern/gl_core_4_4.h"

namespace engine
{
    class RenderableString;

    /*
        This class describes a Spritesheet font (v1), such a font consists of 2 files. the spritesheet (image), and a ,info file organized as follows:
        <file start>
        sheet_width sheet_height
        sprite_width sprite_height
        first_ascii_character
        advance_between_characters
        advance_between_newlines
        <for each character>
        width
        <file end>
        # denotes a comment and should be ignored.
    */
    class Font
    {
        public:
            Font(const std::string &fontname); //searches for *.png & .info itsself.
            Font(const Font&) = delete;
            Font(const Font&&) = delete;
            ~Font();

            bool contains(char c) const; //returns whether this char can be drawn.
            bool valid(const std::string &str); //returns whether string can be drawn.
            RenderableString* renderable_string(const std::string &str);
            void renderable_string(RenderableString *old, const std::string &str); //reuses old buffer for new string.

        protected:
            GLuint m_texture;
            char m_first;
            int m_advance;
            int m_newline_advance;
            int *widths;
            size_t m_sheet_width;
            size_t m_sheet_height;
            size_t m_sprite_width;
            size_t m_sprite_height;

            void parse_info(const std::string &file);
    };

    class RenderableString
    {
    public:
        RenderableString(const RenderableString&) = delete;
        RenderableString(const RenderableString&&) = delete;
        ~RenderableString();

        void draw() const;
        
        //setters/getters
        size_t width() const;
        size_t height() const;
        GLuint texture() const;
        GLuint vertex_buffer() const;
        std::string string() const;

        math::Vector3<float> color() const;
        void color(const math::Vector3<float> &color);
        math::Matrix4x4<float> transform() const;
        void transform(const math::Matrix4x4<float> &transform);
        
    protected:
        friend class Font;
        RenderableString() 
            : m_height(0), m_width(0), m_triangles(0), m_texture(0), m_vertex_buffer(0), m_string(""), m_color(), m_transform() { };

        size_t m_height;
        size_t m_width;
        size_t m_triangles;
        
        GLuint m_texture;
        GLuint m_vertex_array;
        GLuint m_vertex_buffer;
        
        std::string m_string;
        math::Vector3<float> m_color;
        math::Matrix4x4<float> m_transform;
    };

}

#endif