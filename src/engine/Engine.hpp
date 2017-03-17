#ifndef BBM_ENGINE_HPP
#define BBM_ENGINE_HPP

namespace engine
{

    //variables in cpp file but also (sort-of) listed here:
    /// window, opengl context
    //  TextureCache, ShaderProgram and relevant values
    //  list of player objects
    //  Actors (temporary/moving objects)
    //  tickspeed, tick#
    //  current room(majorstate) / tick when room started.
    /// trigger list. std::map<"key", std::pair<tickstamp, "value">>.

    void unload_engine();
    void initialize_engine(); //throws when something fails.

    void game_loop(); //can block a loooong time :)

    void set_default_settings(); //generates and sets default settings into engine.
}

#endif
