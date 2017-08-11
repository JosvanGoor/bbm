#include "Bomberman.hpp"

/////////////////////////////////////////////////////////////////////////
//                          Setters & Getters                          //
/////////////////////////////////////////////////////////////////////////

size_t Bomberman::tick_rate() const
{
    return m_tick_rate;
}

size_t Bomberman::current_tick() const
{
    return m_current_engine_tick;
}

engine::DrawCall& Bomberman::unity_quad()
{
    return *m_unity_quad;
}

engine::Font& Bomberman::default_font()
{
    return *m_default_font;
}

engine::TextureCache& Bomberman::texture_cache()
{
    return m_texture_cache;
}

utility::Settings& Bomberman::settings()
{
    return m_settings;
}

math::Matrix4f Bomberman::projection() const
{
    return m_projection;
}

math::Matrix4f Bomberman::default_view() const
{
    return m_default_view;
}

engine::ShaderProgram* Bomberman::shader_program()
{
    return m_shader_program;
}

GLuint Bomberman::shloc_texture() const
{
    return m_shloc_texture;
}

GLuint Bomberman::shloc_translation() const
{
    return m_shloc_translation;
}

GLuint Bomberman::shloc_view() const
{
    return m_shloc_view;
}

GLuint Bomberman::shloc_projection() const
{
    return m_shloc_projection;
}

GLuint Bomberman::shloc_color_filter() const
{
    return m_shloc_color_filter;
}

void Bomberman::quit()
{
    m_game_running = false;
}