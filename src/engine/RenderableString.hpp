#ifndef ENGINE_RENDERABLE_STRING_HPP
#define ENGINE_RENDERABLE_STRING_HPP

#include "DrawCall.hpp"
class Font;

namespace engine
{

    class RenderableString : public DrawCall
    {
        public:
            RenderableString(const RenderableString&) = delete;
            RenderableString(RenderableString&&) = delete;
            void operator=(const RenderableString&) = delete;
            void operator=(RenderableString&&) = delete;
            

            size_t width() const;
            size_t height() const;
            std::string string() const;
        
        protected:
            friend class Font;
            RenderableString(const std::vector<GLfloat> &data, const std::string &tex) :
                DrawCall(data, tex) {};

            size_t m_width;
            size_t m_height;
            std::string m_string;
    };

}

#endif