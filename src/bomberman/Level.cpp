#include "Level.hpp"

#include "actors/Scenery.hpp"
#include "actors/Destructables.hpp"
#include "../engine/Bomberman.hpp"

namespace engine
{

    Level::Level()
    {
        m_gridsize = 64;
        m_width = 20;
        m_height = 14; //just some initial whatevers.
        m_triangles = 0;
    }

    Level::~Level()
    {
        for(Actor *a : m_scenery)
        {
            delete a;
        }

        for(Actor *a : m_actors)
        {
            delete a;
        }

        m_scenery.clear();
        m_actors.clear();
    }


    size_t Level::gridsize() const { return m_gridsize; }
    void Level::gridsize(size_t gs) { m_gridsize = gs; }
    size_t Level::width() const { return m_width; }
    size_t Level::pix_width() const { return m_width * m_gridsize; }
    void Level::width(size_t w) { m_width = w;}
    size_t Level::height() const { return m_height; }
    size_t Level::pix_height() const { return m_height * m_gridsize; }
    void Level::height(size_t h) { m_height = h; }

    size_t Level::actor_count() const { return m_actors.size(); }
    size_t Level::scenery_count() const { return m_scenery.size(); }

    void Level::add_actor(Actor *a) { m_actors.push_back(a); }
    void Level::add_scenery(Actor *a) { m_scenery.push_back(a); }

    void Level::prepare_scene_buffer()
    {
        std::vector<GLfloat> floats;
        floats.reserve(m_scenery.size() * 2 * 5 * 3);
        m_triangles = 0;

        for(Actor *a : m_scenery)
        {
            std::array<float, 8> tex = a->texture_coordinates();
            geometry::Rectangle pos = a->position();

            floats.insert(floats.end(), {(float)pos.x(), (float)pos.y(), 0, tex[0], tex[1]});
            floats.insert(floats.end(), {(float)pos.x() + pos.width(), (float)pos.y(), 0, tex[2], tex[3]});
            floats.insert(floats.end(), {(float)pos.x(), (float)pos.y() + pos.height(), 0, tex[4], tex[5]});
            floats.insert(floats.end(), {(float)pos.x() + pos.width(), (float)pos.y(), 0, tex[2], tex[3]});
            floats.insert(floats.end(), {(float)pos.x() + pos.width(), (float)pos.y() + pos.height(), 0, tex[6], tex[7]});
            floats.insert(floats.end(), {(float)pos.x(), (float)pos.y() + pos.height(), 0, tex[4], tex[5]});
            m_triangles += 2;
        }

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * floats.size(), floats.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        int stride = (3 + 2) * sizeof(GLfloat);
        void *voffset = (void*)0;
        void *toffset = (void*)(3 * sizeof(GLfloat));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, voffset);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, toffset);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //draw in the middle.
        int screen_width = Bomberman::instance().drawspace_width();
        int screen_height = Bomberman::instance().drawspace_height();

        m_offset.x(screen_width / 2 - pix_width() / 2);
        m_offset.y(screen_height / 2 - pix_height() / 2);
    }

    Room *Level::logic_update()
    {
        //collisionchecking
        //update players.
        //act for all effects

        return nullptr;
    }

    //for the default level, just sink all events
    void Level::process_events()
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            Bomberman::instance().event_sink(event);
        }
    }

    void Level::draw(double ahead)
    {
        glBindVertexArray(m_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Bomberman::instance().texture_cache().get_texture("textures/brick.png"));

        glUniform3f(Bomberman::instance().shloc_color_filter(), 1.0, 1.0, 1.0); //no filter

        math::Matrix4x4<float> transform;
        transform = transform.translate(m_offset.x(), m_offset.y(), 0);
        glUniformMatrix4fv(Bomberman::instance().shloc_translation(), 1, GL_FALSE, transform.data());
        glUniformMatrix4fv(Bomberman::instance().shloc_projection(), 1, GL_FALSE, Bomberman::instance().projection().data());
        
        glDrawArrays(GL_TRIANGLES, 0, 3 * m_triangles);
        glBindVertexArray(0);


        for(Actor *a : m_actors)
        {
            a->draw(ahead, m_offset.x(), m_offset.y());
        }
    }


    Level* get_testing_level()
    {
        Level *lvl = new Level();
        geometry::Rectangle rect;

        for(size_t i = 0; i < lvl->height(); ++i)
        {
            Brick *br = new Brick();
            rect.x(0);
            rect.y(i * lvl->gridsize());
            rect.width(lvl->gridsize());
            rect.height(lvl->gridsize());
            br->position(rect);
            lvl->add_scenery(br);

            br = new Brick();
            rect.x(lvl->pix_width() - lvl->gridsize());
            br->position(rect);
            lvl->add_scenery(br);
        }

        for(size_t i = 1; i < lvl->width() - 1; ++i)
        {
            Brick *br = new Brick();
            rect.y(0);
            rect.x(i * lvl->gridsize());
            rect.width(lvl->gridsize());
            rect.height(lvl->gridsize());
            br->position(rect);
            lvl->add_scenery(br);

            br = new Brick();
            rect.y(lvl->pix_height() - lvl->gridsize());
            br->position(rect);
            lvl->add_scenery(br);
        }

        Mud *m = new Mud();
        m->position(geometry::Rectangle(3 * lvl->gridsize(), 1 * lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        m = new Mud();
        m->position(geometry::Rectangle(4 * lvl->gridsize(), 1 * lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        m = new Mud();
        m->position(geometry::Rectangle(5 * lvl->gridsize(), 1 * lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        m = new Mud();
        m->position(geometry::Rectangle(3 * lvl->gridsize(), 2 * lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        m = new Mud();
        m->position(geometry::Rectangle(3 * lvl->gridsize(), 3 * lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        m = new Mud();
        m->position(geometry::Rectangle(4 * lvl->gridsize(), 2 * lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        m = new Mud();
        m->position(geometry::Rectangle(10 * lvl->gridsize(), 10 * lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        m = new Mud();
        m->position(geometry::Rectangle(13 * lvl->gridsize(), 10 * lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        m = new Mud();
        m->position(geometry::Rectangle(12 * lvl->gridsize(), 11 * lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        m = new Mud();
        m->position(geometry::Rectangle(11 * lvl->gridsize(), 3* lvl->gridsize(), lvl->gridsize(), lvl->gridsize()));
        lvl->add_actor(m);

        lvl->prepare_scene_buffer();
        std::cout << "\nGenerated testlvl with " << lvl->scenery_count() << " scenery actors.\n";
        std::cout << "Generated testlvl with " << lvl->actor_count() << " actor actors.\n";
        return lvl;
    }
    
}