#include "Bomberman.hpp"

/////////////////////////////////////////////////////////////////////////
//                          Setters & Getters                          //
/////////////////////////////////////////////////////////////////////////

size_t Bomberman::tick_rate()
{
    return m_tick_rate;
}

size_t Bomberman::current_tick()
{
    return m_current_engine_tick;
}

engine::Font* Bomberman::default_font()
{
    return m_default_font;
}

engine::TextureCache& Bomberman::texture_cache()
{
    return m_texture_cache;
}

utility::Settings& Bomberman::settings()
{
    return m_settings;
}

math::Matrix4f Bomberman::projection()
{
    return m_projection;
}

math::Matrix4f Bomberman::default_view()
{
    return m_default_view;
}

engine::ShaderProgram* Bomberman::shader_program()
{
    return m_shader_program;
}

GLuint Bomberman::shloc_texture()
{
    return m_shloc_texture;
}

GLuint Bomberman::shloc_translation()
{
    return m_shloc_translation;
}

GLuint Bomberman::shloc_view()
{
    return m_shloc_view;
}

GLuint Bomberman::shloc_projection()
{
    return m_shloc_projection;
}

GLuint Bomberman::shloc_color_filter()
{
    return m_shloc_color_filter;
}