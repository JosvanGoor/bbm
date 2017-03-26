#include "Bomberman.hpp"

#include <chrono>

#include "Actor.hpp"
#include "../math/Matrix4x4.hpp"

namespace engine
{

    /////////////////////////////////////////////////////////////
    // Game Loop
    /////////////////////////////////////////////////////////////

    void Bomberman::game_loop()
    {
        std::cout << "----- Entering gameloop -----\n";
        m_current_engine_tick = 0; //roomtick 0
        m_tick_duration = 1000.0 / m_tick_rate;

        auto current_time = std::chrono::high_resolution_clock::now();
        auto previous_time = std::chrono::high_resolution_clock::now();
        double lag = 0.0;

        double in_this_second = 0.0;
        size_t updates = 0;
        size_t render_passes = 0;
        std::cout << "Ended timing preparations, entering loop...\n";

        m_game_loop_running = true;
        while(m_game_loop_running)
        {
            current_time = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(current_time - previous_time).count();
            lag += elapsed;
            previous_time = current_time;

            in_this_second += elapsed;
            if(in_this_second >= 1000)
            {
                
                std::cout << "\rfps / logic - " << render_passes << " / " << updates << std::flush; 

                updates = render_passes = 0;
                in_this_second -= 1000;
            }

            /////////////////////////////////////// PROCESS INPUTS /////////////////////////////////////////
            //VOOR NU EIGENLIJK MOETEN ROOMS ZELF INPUT PARSEN
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    m_game_loop_running = false;
                }
            }

            while(lag >= m_tick_duration)
            {
                /////////////////////////////////////// LOGIC UPDATES /////////////////////////////////////////
                m_active_room->logic_update();

                lag -= m_tick_duration;
                updates++;
                m_current_engine_tick++;
            }

            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //----- rendering code -----
            m_active_room->draw(0);

            Actor a;
            a.position().x(100);
            a.position().y(100);
            a.position().width(256);
            a.position().height(256);

            a.draw(0.0);

            //----- rendering code -----

            SDL_GL_SwapWindow(m_window_handle);

            //std::this_thread::sleep_for(std::chrono::milliseconds(50));
            render_passes++;
        }

        std::cout << "\n----- finished gameloop, bye bye :-) -----\n" << std::endl;
    }

    /////////////////////////////////////////////////////////////
    // Convenience
    /////////////////////////////////////////////////////////////

    void Bomberman::end_game() { m_game_loop_running = false; }

    void Bomberman::push_trigger(const std::string &key, const std::string &val)
    {
        m_triggers[key] = std::make_pair(m_current_engine_tick, val);
    }

    std::pair<size_t, std::string> Bomberman::get_recent_trigger(std::string key, size_t maxdelay)
    {
        auto it = m_triggers.find(key);
        if(it != m_triggers.end() && (m_current_engine_tick - maxdelay) <= it->second.first)
            return it->second;
        return std::make_pair((size_t)0, "");
    }

    void Bomberman::draw_quad(const Rectangle &pos, GLuint texture)
    {
        glBindVertexArray(m_quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        math::Matrix4x4<float> transform;
        transform = transform.scale(pos.width(), pos.height(), 0);
        transform = transform.translate(pos.x(), pos.y(), 0);

        glUniformMatrix4fv(m_shloc_translation, 1, GL_FALSE, transform.data());
        glUniformMatrix4fv(m_shloc_projection, 1, GL_FALSE, m_projection.data());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /////////////////////////////////////////////////////////////
    // Setters & Getters
    /////////////////////////////////////////////////////////////

    //fuckton of setters/getters
    bool Bomberman::legacy_mode() const { return m_legacy_mode; }
    bool Bomberman::game_loop_running() const { return m_game_loop_running; }
    size_t Bomberman::tick_rate() const { return m_tick_rate; }
    size_t Bomberman::start_room_tick() const { return m_start_room_tick; }
    size_t Bomberman::current_engine_tick() const { return m_current_engine_tick; }

    Settings& Bomberman::settings() { return m_settings; }
    TextureCache& Bomberman::texture_cache() { return m_texture_cache; }
    std::map<std::string, std::pair<size_t, std::string>>& Bomberman::triggers()
        { return m_triggers; }

    GLuint Bomberman::shloc_projection() const { return m_shloc_projection; }
    GLuint Bomberman::shloc_translation() const { return m_shloc_translation; }
    GLuint Bomberman::shloc_texture() const { return m_shloc_texture; }

    size_t Bomberman::drawspace_width() const { return m_drawspace_width; }
    size_t Bomberman::drawspace_heigt() const {return m_drawspace_height; }

    /////////////////////////////////////////////////////////////
    // (De)constructors & initializers
    /////////////////////////////////////////////////////////////

    Bomberman::Bomberman()
    {
        std::cout << "----- Bomberman engine starting initialization ------\n";
        m_game_loop_running = false;

        std::cout << "Loading default settings set...\n";
        load_default_settings();
        m_settings.load_from_file();
        std::cout << m_settings.full_settings_report("    ");

        std::cout << "Setting tickrate to 40\n";
        m_tick_rate = 40;

        windowing_setup();
        shader_setup();
        basic_draw_setup();

        std::cout << "Setting up mainmenu...\n";
        m_active_room = new MainMenu();

        std::cout << "----- Bomberman engine initialization completed succesfully -----\n" << std::endl;
    }

    Bomberman::~Bomberman()
    {
        std::cout << "----- Started to unload the engine -----\n";

        m_settings.save_to_file();
        std::cout << "Written settings to disk.\n";

        m_texture_cache.clear();
        std::cout << "cleared all textures from memory.\n";

        delete m_shader_program;
        std::cout << "Marked shader program for deconstruction.\n";

        SDL_GL_DeleteContext(m_opengl_context);
        SDL_DestroyWindow(m_window_handle);
        SDL_Quit();
        std::cout << "Destroyed sdl window and quit environment\n";

        std::cout << "----- finished unloading engine -----" << std::endl;
    }

    void Bomberman::windowing_setup()
    {
        std::cout << "Starting windowing setup...\n";

        size_t x, y, w, h;
        x = m_settings.as_int("screen_x");
        y = m_settings.as_int("screen_y");
        w = m_settings.as_int("screen_width");
        h = m_settings.as_int("screen_height");

        m_window_handle = SDL_CreateWindow("Bomberman v0.1", x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        m_projection = math::orthographic_projection(0.0f, 1920.0f, 1080.0f, .0f, -1.0f, 1.0f);
        m_drawspace_width = 1920; m_drawspace_height = 1080;
        if(!m_window_handle) throw Exception(__PRETTY_FUNCTION__, "could not create sdl window");
        std::cout << "    Created window...\n";

        m_opengl_context = SDL_GL_CreateContext(m_window_handle);
        if(!m_opengl_context) throw Exception(__PRETTY_FUNCTION__, "cound not create opengl context for sdl window");
        std::cout << "    Set openGL context...\n";

        if(ogl_LoadFunctions() == ogl_LOAD_FAILED) throw Exception(__PRETTY_FUNCTION__, "failed to load opengl extentions (version 4.4 requested)");
        std::cout << "    Loaded openGL functions and extentions...\n";

        SDL_GL_SetSwapInterval(m_settings.as_int("vsyncmode")); //disable vsync
        std::cout << "    Disabled vertical sync...\nWindow setup complete!" << std::endl;
    }

    void Bomberman::shader_setup()
    {
        std::cout << "Starting shader setup...\n";

        m_shader_program = ShaderProgram::create();
        try 
        {
            std::cout << "    Attempting to compile shaders... ";
            m_shader_program->add_vertex_shader("shaders/vertexshader2d.glsl");
            m_shader_program->add_fragment_shader("shaders/fragmentshader2d.glsl");
            std::cout << "done!\n";
        }
        catch(Exception &ex)
        {
            std::cout << "failed\n        Reason: " << ex.message();
            std::cout << "    Attempting legacy shaders... ";
            m_shader_program->add_vertex_shader("shaders/vertexshader2dlegacy.glsl");
            m_shader_program->add_fragment_shader("shaders/fragmentshader2dlegacy.glsl");
            glBindAttribLocation(m_shader_program->program_id(), 0, "position");
            glBindAttribLocation(m_shader_program->program_id(), 1, "texture_coords");
            m_legacy_mode = true;
            std::cout << "done\n";
        }

        m_shader_program->link();
        m_shader_program->activate();
        std::cout << "    Shaders linked and activated\n";

        m_shloc_projection = m_shader_program->uniform_location("ortho");
        m_shloc_texture = m_shader_program->uniform_location("tex");
        m_shloc_translation = m_shader_program->uniform_location("transform");
        std::cout << "    Retrieved shader's data locations, Shader setup complete!" << std::endl;
    }

    void Bomberman::basic_draw_setup()
    {
        std::cout << "Enable simple quad drawing...\n";

        GLfloat floats[] = {0.0, 0.0, 0.0,      0.0, 0.0,
                            1.0, 0.0, 0.0,      1.0, 0.0,
                            0.0, 1.0, 0.0,      0.0, 1.0,
                            1.0, 0.0, 0.0,      1.0, 0.0,
                            1.0, 1.0, 0.0,      1.0, 1.0,
                            0.0, 1.0, 0.0,      0.0, 1.0};
        
        std::cout << "Generating VAO...\n";
        glGenVertexArrays(1, &m_quad_vao);
        glBindVertexArray(m_quad_vao);
        
        std::cout << "Generate VBO...\n";
        glGenBuffers(1, &m_quad_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(floats), floats, GL_STATIC_DRAW);
        std::cout << "VBO created and populated (vvvtt interleaved).\n";

        std::cout << "Setting VAO draw info...\n";
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        int stride = (3 + 2) * sizeof(GLfloat);
        void *voffset = (void*)0;
        void *toffset = (void*)(3 * sizeof(GLfloat));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, voffset);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, toffset);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::cout << "quad drawing setup finished." << std::endl;
    }

    void Bomberman::load_default_settings()
    {
        m_settings.set("vsyncmode", "0");
        m_settings.set("screen_x", "200");
        m_settings.set("screen_y", "200");
        m_settings.set("screen_width", "1366");
        m_settings.set("screen_height", "768");
    }
}