#ifndef BOMBERMAN_LEVEL_HPP
#define BOMBERMAN_LEVEL_HPP

#include <vector>
#include "entities/Wall.hpp"
#include "entities/Player.hpp"
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

        std::vector<engine::Entity*>& actors();
        std::vector<engine::Entity*>& scenery();

        bool collides_with_actor_type(const geometry::Rectanglef &pos, int type);
        bool collides_with_mud(const geometry::Rectanglef &pos);
        bool collides_with_scenery(const geometry::Rectanglef &pos);

        void debug_print_actors();

        virtual GameStateController* logic_update() override;
        virtual void draw_to_screen(float ahead) override;
        virtual std::string to_string() const override;

    protected:
        engine::DrawCall *m_draw_scenery;

        Player *m_player_1;
        Player *m_player_2;
        Player *m_player_3;
        Player *m_player_4;
        std::vector<engine::Entity*> m_actors;
        std::vector<engine::Entity*> m_scenery;
};

Level* get_default_level();

#endif