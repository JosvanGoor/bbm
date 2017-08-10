#ifndef ENGINE_FONT_HPP
#define ENGINE_FONT_HPP

#include <string>
#include "RenderableString.hpp"

namespace engine
{
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
            Font(const std::string &file);
            Font(const Font&) = delete;
            Font(Font&&) = delete;
            void operator=(const Font&) = delete;
            void operator=(Font&&) = delete;
            ~Font();

            bool contains(char c) const; //returns whether this char can be rendered
            bool is_valid(const std::string &str) const; //returns whetther the complete string can be rendered.
            RenderableString *renderable_string(const std::string &str); //generates a renderable string
            void renderable_string(RenderableString *old, const std::string &newstr); //reuses buffers for new string.

        protected:
            std::string m_texture;
            
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

}

#endif