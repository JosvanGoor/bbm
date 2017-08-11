#ifndef BOMBERMAN_MENUS_MAINMENU_HPP
#define BOMBERMAN_MENUS_MAINMENU_HPP

#include "../../math/Matrix4x4.hpp"
#include "../../geometry/Rectangle.hpp"
#include "../../engine/GameStateController.hpp"

class MainMenu : public engine::GameStateController
{
    public:
        const std::string m_tex_start = "textures/menu_start.png";
        const std::string m_tex_quit = "textures/menu_quit.png";

        MainMenu();

        virtual engine::GameStateController* logic_update() override;
        virtual void draw_to_screen(float ahead) override;

        virtual std::string to_string() const override;

    protected:
        geometry::Rectanglef m_rect_start;
        geometry::Rectanglef m_rect_quit;


};

#endif