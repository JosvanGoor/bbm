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

engine::MouseController& Bomberman::mouse()
{
    return m_mouse;
}

engine::KeyboardController& Bomberman::keyboard_controller()
{
    return m_keyboard;
}

engine::GamepadController& Bomberman::gamepad_controller_1()
{
    return m_gamepad_1;
}

engine::GamepadController& Bomberman::gamepad_controller_2()
{
    return m_gamepad_2;
}

engine::GamepadController& Bomberman::gamepad_controller_3()
{
    return m_gamepad_3;
}

engine::GamepadController& Bomberman::gamepad_controller_4()
{
    return m_gamepad_4;
}

engine::Controller& Bomberman::get_named_controller(engine::NamedController nc)
{
    switch(nc)
    {
        case engine::GAMEPAD_1: return m_gamepad_1;
        case engine::GAMEPAD_2: return m_gamepad_2;
        case engine::GAMEPAD_3: return m_gamepad_3;
        case engine::GAMEPAD_4: return m_gamepad_4;
        case engine::KEYBOARD_1: return m_keyboard;
        default: throw Exception(__PRETTY_FUNCTION__, "Enumerated controller not recognized by the engine.");
    }
}

engine::ShaderProgram* Bomberman::shader_program() const
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