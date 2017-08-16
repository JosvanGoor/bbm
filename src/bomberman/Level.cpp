#include "Level.hpp"

Level::Level()
{
    //?
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
    e->bind(this);
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

//TODO: add players
engine::GameStateController* Level::logic_update()
{
    //call act on everything + check for erase
    //m_player_1.act();
    //m_player_2.act();
    //m_player_3.act();
    //m_player_4.act();

    //update all actors
    for(engine::Entity* e : m_actors)
    {
        e->act();
    }

    //collision check everything
    for(auto it = m_actors.begin(); it != m_actors.end(); ++it)
    {
        for(auto it2 = it; it2 != m_actors.end(); ++it2)
        {
            if(it == it2) continue;
            if((*it)->position().intersects((*it2)->position()))
            {
                (*it)->collision(*it2);
                (*it2)->collision(*it);
            }
        }
    }

    //handle removals (this should be done faster/earlier)
    for(auto it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->marked_for_deletion())
            it = m_actors.erase(it);
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
}

std::string Level::to_string() const
{
    return "bomberman::entities::Level";
}

Level* get_default_level()
{
    using geometry::Rectanglef;

    Level *rval = new Level();

    rval->add_scenery(new Wall("/textures/brick.png", Rectanglef(0, 0, 48, 48)));
    rval->add_scenery(new Wall("/textures/brick.png", Rectanglef(48, 0, 48, 48)));
    rval->add_scenery(new Wall("/textures/brick.png", Rectanglef(96, 0, 48, 48)));
    rval->add_scenery(new Wall("/textures/brick.png", Rectanglef(0, 48, 48, 48)));

    rval->setup_scenery_drawcall();
    return rval;
}