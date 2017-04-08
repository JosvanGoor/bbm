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
        m_last_fps = m_last_lps = 0;
        std::cout << "Ended timing preparations...\n";

        std::cout << "Loading default font...\n";
        m_font = new Font("textures/font");

        RenderableString *fps_count = m_font->renderable_string(to_uppercase("LPS/FPS: ?/?"));
        {
            math::Matrix4x4<float> mat;
            mat = mat.translate((float)1, m_drawspace_height - 17.0f, (float)0);
            fps_count->transform(mat);
            fps_count->color(math::Vector3<float>(0.0, 1.0, 0.0));
        } //mat gets removed from stack.
        std::cout << "Finished setting up fps-counter...\n";

        std::cout << "Setting up mainmenu...\n";
        m_active_room = new MainMenu();
        std::cout << "Finished final preparations, entering gameloop.\n";


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
                m_font->renderable_string(fps_count, to_uppercase("LPS/FPS: " + std::to_string(updates) + "/" + std::to_string(render_passes)));
                
                m_last_fps = render_passes;
                m_last_lps = updates;

                updates = render_passes = 0;
                in_this_second -= 1000;
            }

            /////////////////////////////////////// PROCESS INPUTS /////////////////////////////////////////
            m_active_room->process_events();

            while(lag >= m_tick_duration)
            {
                /////////////////////////////////////// LOGIC UPDATES /////////////////////////////////////////
                Room *buf = m_active_room->logic_update();
                if(buf != nullptr) //swap room
                {
                    delete m_active_room;
                    m_active_room = buf;
                }

                //TEST REMOVE THIS
                //std::cout << "KB1: " << m_keyboard_1->state_to_string() << std::endl;
                //std::cout << "GP1: " << m_gamepad_1->state_to_string() << std::endl;
                //std::cout << "GP2: " << m_gamepad_2->state_to_string() << std::endl;
                //std::cout << "GP3: " << m_gamepad_3->state_to_string() << std::endl;
                //std::cout << "GP4: " << m_gamepad_4->state_to_string() << std::endl;

                lag -= m_tick_duration;
                updates++;
                m_current_engine_tick++;
            }

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //----- rendering code -----
            m_active_room->draw(lag / m_tick_duration);
            //----- rendering code -----

            //----- Draw fps counter -----
            fps_count->draw();

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

    void Bomberman::draw_quad(const geometry::Rectangle &pos, GLuint texture)
    {
        glBindVertexArray(m_quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        math::Matrix4x4<float> transform;
        transform = transform.scale(pos.width(), pos.height(), 0);
        transform = transform.translate(pos.x(), pos.y(), 0);

        glUniform3f(m_shloc_color_filter, 1.0, 1.0, 1.0); //always bind white here
        glUniformMatrix4fv(m_shloc_translation, 1, GL_FALSE, transform.data());
        glUniformMatrix4fv(m_shloc_projection, 1, GL_FALSE, m_projection.data());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Controller* Bomberman::controller(NamedController nc)
    {
        switch(nc)
        {
            case GAMEPAD_1: return m_gamepad_1;
            case GAMEPAD_2: return m_gamepad_2;
            case GAMEPAD_3: return m_gamepad_3;
            case GAMEPAD_4: return m_gamepad_4;
            case KEYBOARD_1: return m_keyboard_1;
            default:
                throw Exception(__PRETTY_FUNCTION__, "Requested controller slot not available");
        }
    }

    /////////////////////////////////////////////////////////////
    // Event sink
    /////////////////////////////////////////////////////////////

    void Bomberman::event_sink(const SDL_Event &event)
    {
        switch(event.type)
        {
            case SDL_QUIT:
                end_game();
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                m_keyboard_1->update(event);
                break;
            case SDL_CONTROLLERAXISMOTION:
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
            case SDL_CONTROLLERDEVICEREMOVED:
                m_gamepad_1->update(event);
                m_gamepad_2->update(event);
                m_gamepad_3->update(event);
                m_gamepad_4->update(event);
                break;
            case SDL_CONTROLLERDEVICEADDED:
                if(!m_gamepad_1->connected()) 
                {
                    delete m_gamepad_1;
                    m_gamepad_1 = new GamepadController(SDL_GameControllerOpen(event.cdevice.which));
                    std::cout << "\rNew controller mapped to location 1...\n";
                }
                else if(!m_gamepad_2->connected()) 
                {
                    delete m_gamepad_2;
                    m_gamepad_2 = new GamepadController(SDL_GameControllerOpen(event.cdevice.which));
                    std::cout << "\rNew controller mapped to location 2...\n";
                }
                else if(!m_gamepad_3->connected()) 
                {
                    delete m_gamepad_3;
                    m_gamepad_3 = new GamepadController(SDL_GameControllerOpen(event.cdevice.which));
                    std::cout << "\rNew controller mapped to location 3...\n";
                }
                else if(!m_gamepad_4->connected()) 
                {
                    delete m_gamepad_4;
                    m_gamepad_4 = new GamepadController(SDL_GameControllerOpen(event.cdevice.which));
                    std::cout << "\rNew controller mapped to location 4...\n";
                }
        }
        //TODO: manage resize etc.
    }

    /////////////////////////////////////////////////////////////
    // Setters & Getters
    /////////////////////////////////////////////////////////////

    //fuckton of setters/getters
    bool Bomberman::legacy_mode() const { return m_legacy_mode; }
    bool Bomberman::game_loop_running() const { return m_game_loop_running; }
    size_t Bomberman::tick_rate() const { return m_tick_rate; }
    size_t Bomberman::last_fps() const { return m_last_fps; }
    size_t Bomberman::last_lps() const { return m_last_lps; }
    size_t Bomberman::start_room_tick() const { return m_start_room_tick; }
    size_t Bomberman::current_engine_tick() const { return m_current_engine_tick; }

    Font* Bomberman::font() { return m_font; }
    Settings& Bomberman::settings() { return m_settings; }
    TextureCache& Bomberman::texture_cache() { return m_texture_cache; }
    std::map<std::string, std::pair<size_t, std::string>>& Bomberman::triggers()
        { return m_triggers; }

    GLuint Bomberman::quad_vao() const { return m_quad_vao; }
    GLuint Bomberman::quad_vbo() const { return m_quad_vbo; }
    GLuint Bomberman::shloc_projection() const { return m_shloc_projection; }
    GLuint Bomberman::shloc_translation() const { return m_shloc_translation; }
    GLuint Bomberman::shloc_texture() const { return m_shloc_texture; }
    GLuint Bomberman::shloc_color_filter() const { return m_shloc_color_filter; }

    math::Matrix4x4<float> Bomberman::projection() const { return m_projection; }

    size_t Bomberman::drawspace_width() const { return m_drawspace_width; }
    size_t Bomberman::drawspace_height() const {return m_drawspace_height; }

    /////////////////////////////////////////////////////////////
    // (De)constructors & initializers
    /////////////////////////////////////////////////////////////

    Bomberman::Bomberman()
    {
        std::cout << "----- Bomberman engine starting initialization ------\n";
        m_game_loop_running = false;

        SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO);
        //SDL_GameControllerEventState(SDL_ENABLE);

        std::cout << "Loading default settings set...\n";
        load_default_settings();
        m_settings.load_from_file();
        std::cout << m_settings.full_settings_report("    ");

        std::cout << "Setting tickrate to 40\n";
        m_tick_rate = 40;

        std::cout << "Initial setup for controllers...\n";
        m_gamepad_1 = new GamepadController();
        m_gamepad_2 = new GamepadController();
        m_gamepad_3 = new GamepadController();
        m_gamepad_4 = new GamepadController();
        m_keyboard_1 = new KeyboardController(m_settings.as_string("keyboard_map_0"));

        windowing_setup();
        shader_setup();
        basic_draw_setup();

        std::cout << "----- Bomberman engine initialization completed succesfully -----\n" << std::endl;
    }

    Bomberman::~Bomberman()
    {
        std::cout << "----- Started to unload the engine -----\n";

        m_settings.set("keyboard_map_0", m_keyboard_1->mapping());
        m_settings.save_to_file();
        std::cout << "Written settings to disk.\n";

        delete m_active_room;
        std::cout << "released running resources...\n";

        delete m_gamepad_1;
        delete m_gamepad_2;
        delete m_gamepad_3;
        delete m_gamepad_4;
        std::cout << "Cleared gamepad hooks...\n";

        delete m_font;
        std::cout << "Deleted default font...\n";

        m_texture_cache.clear();
        std::cout << "Cleared all textures from memory.\n";

        delete m_shader_program;
        std::cout << "Marked shader program for deconstruction.\n";

        SDL_GL_DeleteContext(m_opengl_context);
        SDL_DestroyWindow(m_window_handle);
        SDL_Quit();
        std::cout << "Destroyed sdl window and quit environment\n";

        std::cout << "Runtime: " << m_current_engine_tick << " ticks.\n";
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

        int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
        if(m_settings.as_int("borderless")) flags |= SDL_WINDOW_BORDERLESS;
        if(x == 0) x = y = SDL_WINDOWPOS_UNDEFINED;

        m_window_handle = SDL_CreateWindow("Bomberman v0.1", x, y, w, h, flags);
        m_projection = math::orthographic_projection(0.0f, (float)w, (float)h, .0f, -1.0f, 1.0f);
        m_drawspace_width = w; m_drawspace_height = h;
        if(!m_window_handle) throw Exception(__PRETTY_FUNCTION__, "could not create sdl window");
        std::cout << "    Created window...\n";

        m_opengl_context = SDL_GL_CreateContext(m_window_handle);
        if(!m_opengl_context) throw Exception(__PRETTY_FUNCTION__, "cound not create opengl context for sdl window");
        std::cout << "    Set openGL context...\n";

        if(ogl_LoadFunctions() == ogl_LOAD_FAILED) throw Exception(__PRETTY_FUNCTION__, "failed to load opengl extentions (version 4.4 requested)");
        std::cout << "    Loaded openGL functions and extentions...\n";

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        SDL_GL_SetSwapInterval(m_settings.as_int("vsyncmode")); //disable vsync
        std::cout << "    Set vertical sync state...\nWindow setup complete!" << std::endl;
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
        m_shloc_color_filter = m_shader_program->uniform_location("cfilter");
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
        
        std::cout << "    Generating VAO...\n";
        glGenVertexArrays(1, &m_quad_vao);
        glBindVertexArray(m_quad_vao);
        
        std::cout << "    Generate VBO...\n";
        glGenBuffers(1, &m_quad_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(floats), floats, GL_STATIC_DRAW);
        std::cout << "    VBO created and populated (vvvtt interleaved).\n";

        std::cout << "    Setting VAO parameters...\n";
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        int stride = (3 + 2) * sizeof(GLfloat);
        void *voffset = (void*)0;
        void *toffset = (void*)(3 * sizeof(GLfloat));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, voffset);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, toffset);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::cout << "quad setup finished." << std::endl;
    }

    void Bomberman::load_default_settings()
    {
        m_settings.set("vsyncmode", "0");
        m_settings.set("screen_x", "200");
        m_settings.set("screen_y", "200");
        m_settings.set("screen_width", "1366");
        m_settings.set("screen_height", "768");
        m_settings.set("borderless", "1");
        m_settings.set("controller_deadzone", "0.15");
        m_settings.set("keyboard_map_0", "1073741920,1073741917,1073741916,1073741918,13,27,1073742052,1073742054,32,120,113,112,101,114,121,117,1073741906,1073741905,1073741904,1073741903,119,115,97,100");
    }
}