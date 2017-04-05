#include "MainMenu.hpp"

namespace engine
{

    MainMenu::MainMenu()
    {
        int width = Bomberman::instance().drawspace_width() * 0.4;
        int xpos = Bomberman::instance().drawspace_width()/2 - (width / 2);
        int height = width / 4.0;
        int ypos = Bomberman::instance().drawspace_height() / 6.0;

        m_last_click = Point(-1, -1); //-1 signifies no click.

        m_start = new Button("textures/menu_start.png", xpos, ypos, width, height);
        ypos += ypos + height;
        m_quit = new Button("textures/menu_quit.png", xpos, ypos, width, height);
    }

    MainMenu::~MainMenu()
    {
        delete m_start;
        delete m_quit;
    }

    Room* MainMenu::logic_update()
    {
        if(m_last_click.x() == -1) return nullptr;

        if(m_start->clicked(m_last_click)) std::cout << "start clicked :)\n";
        if(m_quit->clicked(m_last_click)) Bomberman::instance().end_game();

        m_last_click = Point(-1, -1);
        return nullptr;
    }

    void MainMenu::process_events()
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONUP:
                    m_last_click = Point(event.button.x, event.button.y);
                    break;
                default:
                    Bomberman::instance().event_sink(event);
            }
            
        }
    }

    void MainMenu::draw(double ahead)
    {
        m_start->draw(0);
        m_quit->draw(0);
    }

}