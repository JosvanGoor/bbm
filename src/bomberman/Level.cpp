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
    m_actors.push_back(e);
}

void Level::add_scenery(engine::Entity* e)
{
    m_scenery.push_back(e);
}

engine::GameStateController* Level::logic_update()
{
    return nullptr;
}

void Level::draw_to_screen(float ahead)
{
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