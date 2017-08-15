#include "MainMenu.hpp"

#include "../Bomberman.hpp"

MainMenu::MainMenu()
{
    int width = Bomberman::instance().settings().as_int("__scr_w");
    int xpos = width / 2 - ((width * 0.4) / 2);
    width *= 0.4;
    int height = width / 4.0;
    int ypos = Bomberman::instance().settings().as_int("__scr_h") / 6.0;

    m_rect_start.set(xpos, ypos, width, height);
    ypos += ypos + height;
    m_rect_quit.set(xpos, ypos, width, height);
}

engine::GameStateController* MainMenu::logic_update()
{
    //check if mouse is clicked on buttons
    if(Bomberman::instance().mouse().left_mouse_button() == engine::BUTTON_RELEASED)
    {
        if(m_rect_start.contains(Bomberman::instance().mouse().cursor_position().as<float>()))
        {
            //start pressed
        }
        
        if(m_rect_quit.contains(Bomberman::instance().mouse().cursor_position().as<float>()))
        {
            //quit pressed
            Bomberman::instance().quit();
        }
    }

    return nullptr;
}

void MainMenu::draw_to_screen(float ahead)
{
    Bomberman::instance().unity_quad().draw(m_rect_start, m_tex_start);
    Bomberman::instance().unity_quad().draw(m_rect_quit, m_tex_quit);
}

std::string MainMenu::to_string() const 
{
    return "class MainMenu : public engine::GameStateController";
}