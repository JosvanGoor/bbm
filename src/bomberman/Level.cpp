#include "Level.hpp"

#include <iostream>
#include <algorithm>

#include "entities/Bomb.hpp"
#include "entities/Mud.hpp"

#include "../engine/Controllers.hpp"

geometry::Rectanglef get_pos_grid(int x, int y)
{
    return geometry::Rectanglef(x * 64, y * 64, 64, 64);
}

Level::Level()
{
    //init players
    m_player_1 = new Player(engine::GAMEPAD_1, get_pos_grid(1, 1));
    m_player_2 = new Player(engine::GAMEPAD_2, get_pos_grid(1, 14));
    m_player_3 = new Player(engine::GAMEPAD_3, get_pos_grid(14, 1));
    m_player_4 = new Player(engine::GAMEPAD_4, get_pos_grid(14, 14));
}

void Level::setup_scenery_drawcall()
{
    std::vector<GLfloat> floats;
    floats.reserve(m_scenery.size() * 2 * 5 * 3);

    std::array<float, 8> tex ={ 0.0, 0.0, //x y
                                1.0, 0.0, //x+w y
                                0.0, 1.0, //x y+h
                                1.0, 1.0 }; //x+w y+h

    for(engine::Entity *a : m_scenery)
    {
        geometry::Rectanglef pos = a->position();
        floats.insert(floats.end(), {(float)pos.x(), (float)pos.y(), 0, tex[0], tex[1]});
        floats.insert(floats.end(), {(float)pos.x() + pos.width(), (float)pos.y(), 0, tex[2], tex[3]});
        floats.insert(floats.end(), {(float)pos.x(), (float)pos.y() + pos.height(), 0, tex[4], tex[5]});
        floats.insert(floats.end(), {(float)pos.x() + pos.width(), (float)pos.y(), 0, tex[2], tex[3]});
        floats.insert(floats.end(), {(float)pos.x() + pos.width(), (float)pos.y() + pos.height(), 0, tex[6], tex[7]});
        floats.insert(floats.end(), {(float)pos.x(), (float)pos.y() + pos.height(), 0, tex[4], tex[5]});
    }

    m_draw_scenery = new engine::DrawCall(floats, "textures/brick.png");
}

void Level::add_actor(engine::Entity* e)
{
    m_actors.push_back(e);
}

void Level::add_scenery(engine::Entity* e)
{
    m_scenery.push_back(e);
}

std::vector<engine::Entity*>& Level::actors()
{
    return m_actors;
}

std::vector<engine::Entity*>& Level::scenery()
{
    return m_scenery;
}

bool Level::collides_with_actor_type(const geometry::Rectanglef &pos, int type)
{
    for(auto it = m_actors.begin(); it != m_actors.end(); ++it)
    {
        if((*it)->type() == type && (*it)->position().intersects(pos)) return true;
    }
    return false;    
}

bool Level::collides_with_mud(const geometry::Rectanglef &pos)
{
    for(auto it = m_actors.begin(); it != m_actors.end(); ++it)
    {
        if((*it)->type() == MUD && (*it)->position().intersects(pos)) return true;
    }
    return false;
}

bool Level::collides_with_scenery(const geometry::Rectanglef &pos)
{
    for(auto it = m_scenery.begin(); it != m_scenery.end(); ++it)
    {
        if((*it)->position().intersects(pos)) return true;
    }
    return false;
}

void Level::debug_print_actors()
{
    for(engine::Entity *e : m_actors)
    {
        std::cout << *e << std::endl;
    }
}

//TODO: add players
engine::GameStateController* Level::logic_update()
{
    //call act on everything + check for erase
    m_player_1->act();
    m_player_2->act();
    m_player_3->act();
    m_player_4->act();

    //update all actors
    for(size_t i = 0; i < m_actors.size(); ++i)
    {
        m_actors[i]->act();
    }

    //collision check everything
    for(size_t i = 0; i < m_actors.size(); ++i)
    {
        if(m_actors[i]->position().intersects(m_player_1->position()))
            m_player_1->collision(m_actors[i]);
        if(m_actors[i]->position().intersects(m_player_2->position()))
            m_player_2->collision(m_actors[i]);
        if(m_actors[i]->position().intersects(m_player_3->position()))
            m_player_3->collision(m_actors[i]);
        if(m_actors[i]->position().intersects(m_player_4->position()))
            m_player_4->collision(m_actors[i]);

        for(size_t j = i+1; j < m_actors.size(); ++j)
        {
            if(m_actors[i]->position().intersects(m_actors[j]->position()))
            {
                m_actors[i]->collision(m_actors[j]);
                m_actors[j]->collision(m_actors[i]);
            }
        }
    }

    size_t i = 0;
    while(i < m_actors.size())
    {
        if(m_actors[i]->marked_for_deletion())
        {
            m_actors.erase(m_actors.begin() + i);
        }
        else i++;
    }
    
    return nullptr;
}

void Level::draw_to_screen(float ahead)
{
    for(engine::Entity* e : m_actors)
    {
        e->draw(ahead);
    }
    m_draw_scenery->draw();

    m_player_1->draw(ahead);
    m_player_2->draw(ahead);
    m_player_3->draw(ahead);
    m_player_4->draw(ahead);
}

std::string Level::to_string() const
{
    return "bomberman::entities::Level";
}

Level* get_default_level()
{
    using geometry::Rectanglef;

    Level *rval = new Level();

    //build wall around
    for(int i = 0; i < 16; ++i)
    {
        rval->add_scenery(new Wall(get_pos_grid(i, 0)));
        rval->add_scenery(new Wall(get_pos_grid(0, i)));
        rval->add_scenery(new Wall(get_pos_grid(i, 15)));
        rval->add_scenery(new Wall(get_pos_grid(15, i)));
    }

    //walls up from spawn
    for(int i = 0; i < 3; ++i)
    {
        rval->add_scenery(new Wall(get_pos_grid(2, 1+i)));
        rval->add_scenery(new Wall(get_pos_grid(13, 1+i)));
        rval->add_scenery(new Wall(get_pos_grid(2, 14-i)));
        rval->add_scenery(new Wall(get_pos_grid(13, 14-i)));
    }

    //first mud
    for(int i = 0; i < 4; ++i)
    {
        rval->add_actor(new Mud(get_pos_grid(3, 1+i)));
        rval->add_actor(new Mud(get_pos_grid(4, 1+i)));

        rval->add_actor(new Mud(get_pos_grid(12, 1+i)));
        rval->add_actor(new Mud(get_pos_grid(11, 1+i)));

        rval->add_actor(new Mud(get_pos_grid(3, 14-i)));
        rval->add_actor(new Mud(get_pos_grid(4, 14-i)));

        rval->add_actor(new Mud(get_pos_grid(12, 14-i)));
        rval->add_actor(new Mud(get_pos_grid(11, 14-i)));

        //top & bot mid line
        rval->add_scenery(new Wall(get_pos_grid(6+i, 2)));
        rval->add_actor(new Mud(get_pos_grid(6+i, 3)));
        rval->add_actor(new Mud(get_pos_grid(6+i, 1)));
        rval->add_scenery(new Wall(get_pos_grid(6+i, 13)));
        rval->add_actor(new Mud(get_pos_grid(6+i, 12)));
        rval->add_actor(new Mud(get_pos_grid(6+i, 14)));

        //leftmid & rightmid dirt
        rval->add_actor(new Mud(get_pos_grid(1, 6+i)));
        rval->add_actor(new Mud(get_pos_grid(14, 6+i)));

        //spawn closers
        rval->add_scenery(new Wall(get_pos_grid(1+i, 5)));
        rval->add_scenery(new Wall(get_pos_grid(1+i, 10)));
        rval->add_scenery(new Wall(get_pos_grid(14-i, 5)));
        rval->add_scenery(new Wall(get_pos_grid(14-i, 10)));
    }

    //mid bit
    for(int i = 0; i < 6; ++i)
    {
        rval->add_actor(new Mud(get_pos_grid(5+i, 7)));
        rval->add_actor(new Mud(get_pos_grid(5+i, 8)));
    }

    for(int i = 0; i < 2; ++i)
    {
        rval->add_scenery(new Wall(get_pos_grid(6, 5+i)));
        rval->add_scenery(new Wall(get_pos_grid(9, 5+i)));
        rval->add_scenery(new Wall(get_pos_grid(6, 9+i)));
        rval->add_scenery(new Wall(get_pos_grid(9, 9+i)));

        rval->add_scenery(new Wall(get_pos_grid(3+i, 7)));
        rval->add_scenery(new Wall(get_pos_grid(3+i, 8)));
        rval->add_scenery(new Wall(get_pos_grid(11+i, 7)));
        rval->add_scenery(new Wall(get_pos_grid(11+i, 8)));

        rval->add_actor(new Mud(get_pos_grid(3+i, 6)));
        rval->add_actor(new Mud(get_pos_grid(3+i, 9)));
        rval->add_actor(new Mud(get_pos_grid(11+i, 6)));
        rval->add_actor(new Mud(get_pos_grid(11+i, 9)));

        rval->add_actor(new Mud(get_pos_grid(7+i, 5)));
        rval->add_actor(new Mud(get_pos_grid(7+i, 6)));
        rval->add_actor(new Mud(get_pos_grid(7+i, 9)));
        rval->add_actor(new Mud(get_pos_grid(7+i, 10)));


    }



    rval->setup_scenery_drawcall();
    return rval;
}