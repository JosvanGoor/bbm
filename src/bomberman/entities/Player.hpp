#ifndef BOMBERMAN_ENTITIES_PLAYER_HPP
#define BOMBERMAN_ENTITIES_PLAYER_HPP

#include "../../engine/Entity.hpp"
#include "../../engine/Controllers.hpp"
#include "../../geometry/Rectangle.hpp"

#define DEADZONE 0.5
#define GRID_SNAP 32

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NO_DIRECTION
};

class Player : public engine::Entity
{
    public:
        Player(engine::NamedController nc, const geometry::Rectanglef &pos);

        int player_id();

        void restore_ammo();

        virtual void act() override;
        virtual void draw(double ahead) override;
        virtual void collision(engine::Entity *entity) override;
    
    private:
        int m_ammo;
        int m_speed;
        int m_player;
        int m_poisoned;
        int m_bomb_power;
        Direction m_direction;
        engine::NamedController m_controller;
};

#endif