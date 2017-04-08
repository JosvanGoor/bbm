#ifndef ENGINE_BOMBERMAN_LEVEL_HPP
#define ENGINE_BOMBERMAN_LEVEL_HPP

#include <vector>
#include "../Room.hpp"
#include "../Actor.hpp"

namespace engine
{

    /*
        A level is the manager of the battlefield the players play in.
    */
    class Level : public Room
    {
    public:
        Level();
        virtual ~Level();

        size_t gridsize() const;
        void gridsize(size_t gs);
        size_t width() const;
        size_t pix_width() const;
        void width(size_t w);
        size_t height() const;
        size_t pix_height() const;
        void height(size_t h);

        size_t actor_count() const;
        size_t scenery_count() const;

        void add_actor(Actor *a);
        void add_scenery(Actor *a);

        void prepare_scene_buffer();
        //void update_scene_buffer(); //OPTIONAL MAYBE WHEN I FEEL LIKE I NEED IT LOL

        virtual Room* logic_update();
        virtual void process_events();
        virtual void draw(double ahead);
    
    protected:
        size_t m_gridsize; //snap to grid value
        size_t m_width;
        size_t m_height;

        //draw optimization scenery.
        GLuint m_vao;
        GLuint m_vbo;
        size_t m_triangles;
        geometry::Point m_offset;

        //players 1-4?
        std::vector<Actor*> m_actors; //rapidly changing
        std::vector<Actor*> m_scenery; //never changes during playtime.

    };

    Level* get_testing_level();

}

#endif