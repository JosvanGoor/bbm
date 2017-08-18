#include "Player.hpp"

#include <iomanip>

#include "Bomb.hpp"
#include "Explosion.hpp"
#include "../Bomberman.hpp"

Player::Player(engine::NamedController nc, const geometry::Rectanglef &pos) : engine::Entity(0)
{
    switch(nc)
    {
        case engine::GAMEPAD_1:
            m_type = PLAYER_1;
            m_player = 1;
            m_texture = "textures/player_blue.png";
            break;
        case engine::GAMEPAD_2:
            m_type = PLAYER_2;
            m_player = 2;
            m_texture = "textures/player_red.png";
            break;
        case engine::GAMEPAD_3:
            m_type = PLAYER_3;
            m_player = 3;
            m_texture = "textures/player_white.png";
            break;
        case engine::GAMEPAD_4:
            m_type = PLAYER_4;
            m_player = 4;
            m_texture = "textures/player_yellow.png";
            break;
        default:
            break;
    }

    m_controller = nc;
    m_position = pos;

    m_ammo = 1;
    m_speed = 6;
    m_poisoned = false;
    m_bomb_power = 2;
    m_direction = NO_DIRECTION;
}

int Player::player_id()
{
    return m_player;
}

void Player::restore_ammo()
{
    m_ammo++;
}

void Player::act()
{
    if(marked_for_deletion()) return;

    if(m_poisoned) m_poisoned--;

    engine::GamepadController *controller = (engine::GamepadController*)Bomberman::instance().get_named_controller(m_controller);

    Direction prev = m_direction;
    if(controller->main_stick_x() > DEADZONE) m_direction = RIGHT;
    else if(controller->main_stick_x() < -DEADZONE) m_direction = LEFT;
    else if(controller->main_stick_y() > DEADZONE) m_direction = DOWN;
    else if(controller->main_stick_y() < -DEADZONE) m_direction = UP;
    else m_direction = NO_DIRECTION;

    if(m_direction != prev)
        m_position.snap_to_grid(GRID_SNAP, GRID_SNAP);
    
    int speed = m_poisoned ? m_speed * -1 : m_speed;

    switch(m_direction)
    {
        case RIGHT: m_position.translate(speed, 0); break;
        case LEFT: m_position.translate(-speed, 0); break;
        case UP: m_position.translate(0, -speed); break;
        case DOWN: m_position.translate(0, speed); break;
        default: break;
    }

    if((controller->x() || controller->a()) && m_ammo > 0)
    {
        geometry::Rectanglef pos = m_position;
        pos.snap_to_grid(GRID_SNAP*2, GRID_SNAP*2);

        Level *level = (Level*)Bomberman::instance().game_state_controller();
        if(!level->collides_with_actor_type(pos, BOMB))
        {
            level->add_actor(new Bomb(this, m_bomb_power, pos));
            m_ammo--;
        }
    }
}

void Player::draw(double ahead)
{
    if(marked_for_deletion()) return;
    Entity::draw(ahead);
}

void Player::collision(engine::Entity *entity)
{
    if(marked_for_deletion()) return;

    int t;
    switch(entity->type())
    {
        case POWERUP_AMMO:
            m_ammo++;
            entity->mark_for_deletion();
            break;
        case POWERUP_POWER:
            m_bomb_power++;
            entity->mark_for_deletion();
            break;
        case POWERUP_SPEED:
            m_speed++;
            entity->mark_for_deletion();
            break;
        case POWERUP_POISON:
            m_poisoned = 200;
            entity->mark_for_deletion();
            break;

        case EXPLOSION:
            mark_for_deletion();
            Bomberman::instance().get_named_controller(m_controller)->simple_rumble(1.0, 750);
            t = Bomberman::instance().current_tick();
            std::cout << "\nPlayer " << m_player << " died from ";
            std::cout << "player " << ((Explosion*)entity)->player_id() << "'s bomb in tick ";
            std::cout << std::setfill('0') << t << ", (" << std::setw(2) << ((t / 40) / 60);
            std::cout << "m " << std::setw(2) << ((t / 40) % 60) << "s)" << std::endl;
            break;

        //case BOMB:
            //boop if others bomb, nothing if own.
        
        case MUD:
        case WALL:
            switch(m_direction)
            {
                case UP:
                    if(!m_poisoned)
                        m_position.y(entity->position().y() + entity->position().height());
                    else 
                        m_position.y(entity->position().y() - m_position.height());
                    break;
                case DOWN:
                    if(!m_poisoned)
                        m_position.y(entity->position().y() - m_position.height());
                    else
                        m_position.y(entity->position().y() + entity->position().height());
                    break;
                case LEFT:
                    if(!m_poisoned)
                        m_position.x(entity->position().x() + entity->position().width());
                    else
                        m_position.x(entity->position().x() - m_position.width());    
                    break;
                case RIGHT:
                    if(!m_poisoned)
                        m_position.x(entity->position().x() - m_position.width());
                    else
                        m_position.x(entity->position().x() + entity->position().width());
                    break;
                default:
                    m_position.snap_to_grid(m_position.width(), m_position.height());
            }
            break;
    }
}