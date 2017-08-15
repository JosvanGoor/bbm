#ifndef BOMBERMAN_LEVEL_HPP
#define BOMBERMAN_LEVEL_HPP

#include <vector>
#include "entities/Wall.hpp"
#include "../engine/Entity.hpp"
#include "../engine/DrawCall.hpp"
#include "../engine/GameStateController.hpp"
#include "../geometry/Rectangle.hpp"

class Level : public engine::GameStateController
{
    public:
        Level();

        void setup_scenery_drawcall();

        void add_actor(engine::Entity* e);
        void add_scenery(engine::Entity* e);

        virtual GameStateController* logic_update() override;
        virtual void draw_to_screen(float ahead) override;
        virtual std::string to_string() const override;

    protected:
        engine::DrawCall *m_draw_scenery;

        //player 1-4
        std::vector<engine::Entity*> m_actors;
        std::vector<engine::Entity*> m_scenery;
};

Level* get_default_level();

#endif