#include "RenderableString.hpp"

namespace engine
{

    size_t RenderableString::width() const
    {
        return m_width;
    }

    size_t RenderableString::height() const
    {
        return m_height;
    }

    std::string RenderableString::string() const
    {
        return m_string;
    }

}