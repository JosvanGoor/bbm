#include "Bomberman.hpp"

#include <ctime>

////////////////////////////////////////////////////////////////////////
//                            Initializers                            //
////////////////////////////////////////////////////////////////////////

Bomberman::Bomberman()
{
    std::cout << "----- Bomberman engine starting initialization -----\n";

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);

    std::cout << "Loading settings...\n";
    load_default_settings();
    m_settings.load_from_file();
    std::cout << m_settings.full_settings_report("    ");

    std::cout << "Tickrate: 40\n";
    m_tick_rate = 40;

    std::cout << "Seeding int and real random generators\n";
    std::random_device rd;
    srand(time(NULL));
    m_int_random = std::mt19937(time(NULL));
    m_real_random = std::mt19937(time(NULL));

    opengl_setup();
    shader_setup();

    m_game_running = false;

    std::vector<GLfloat> floats = { 0.0, 0.0, 0.0,      0.0, 0.0,
                                    1.0, 0.0, 0.0,      1.0, 0.0,
                                    0.0, 1.0, 0.0,      0.0, 1.0,
                                    1.0, 0.0, 0.0,      1.0, 0.0,
                                    1.0, 1.0, 0.0,      1.0, 1.0,
                                    0.0, 1.0, 0.0,      0.0, 1.0};

    m_unity_quad = new engine::DrawCall(floats, "");

    std::cout << "---- Bomberman engine has succesfully initialized.\n" << std::endl;
}

void Bomberman::opengl_setup()
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

    m_window_handle = SDL_CreateWindow("Bomberman alpha", x, y, w, h, flags);
    m_projection = math::orthographic_projection(.0f, (float)w, (float)h, .0f, -1.0f, 1.0f);
    //m_projection = math::orthographic_projection(.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 0.0f);
    m_settings.set("__scr_h", std::to_string(h));
    m_settings.set("__scr_w", std::to_string(w));

    m_default_view = math::Matrix4f();
    if(!m_window_handle)
        throw Exception(__PRETTY_FUNCTION__, "could not create sdl window.");
    std::cout << "    Created window...\n";

    m_opengl_context = SDL_GL_CreateContext(m_window_handle);
    if(!m_opengl_context) 
        throw Exception(__PRETTY_FUNCTION__, "failed to create an opengl context");
    std::cout << "    Created opengl context...\n";

    if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
        throw Exception(__PRETTY_FUNCTION__, "Failed to load opengl functions and extentions (4.4 attempted)");
    std::cout << "    Loaded opengl functions and extentions...\n";

    glEnable(GL_BLEND);
    glClearColor(0.25, 0.25, 0.25, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_GL_SetSwapInterval(m_settings.as_int("vsyncmode"));
    std::cout << "Windowing setup complete!" << std::endl;
}

void Bomberman::shader_setup()
{
    std::cout << "Building single-pass shaders...\n";
    m_shader_program = engine::ShaderProgram::create();
    try 
    {
        std::cout << "    Attempting to compile shaders... ";
        m_shader_program->add_vertex_shader("shaders/vertexshader2d.glsl");
        m_shader_program->add_fragment_shader("shaders/fragmentshader2d.glsl");
        m_settings.set("__legacy", "0");
        std::cout << "done!\n";
    }
    catch(Exception &ex)
    {
        std::cout << "failed\n        Reason: " << ex.message();
        std::cout << "    Attempting to compile legacy shaders...";
        m_shader_program->add_vertex_shader("shaders/vertexshader2dlegacy.glsl");
        m_shader_program->add_fragment_shader("shaders/fragmentshader2dlegacy.glsl");
        glBindAttribLocation(m_shader_program->program_id(), 0, "position");
        glBindAttribLocation(m_shader_program->program_id(), 1, "texture_coords");
        m_settings.set("__legacy", "1");
        std::cout << "done\n";
    }

    m_shader_program->link();
    m_shader_program->activate();
    std::cout << "    Shaders linked and activated.\n";

    m_shloc_projection = m_shader_program->uniform_location("ortho");
    m_shloc_view = m_shader_program->uniform_location("view");
    m_shloc_texture = m_shader_program->uniform_location("tex");
    m_shloc_translation = m_shader_program->uniform_location("transform");
    m_shloc_color_filter = m_shader_program->uniform_location("cfilter");
    std::cout << "Retrieved shader's data locations, Shader setup complete!" << std::endl;
}

void Bomberman::load_default_settings()
{
    m_settings.set("vsyncmode", "0");
    m_settings.set("screen_x", "200");
    m_settings.set("screen_y", "200");
    m_settings.set("screen_width", "1366");
    m_settings.set("screen_height", "768");
    m_settings.set("borderless", "0");
    m_settings.set("controller_deadzone", "0.15");
}

Bomberman::~Bomberman()
{
    std::cout << "----- Started unloading the engine -----\n";
    m_settings.save_to_file();
    std::cout << "Written settings to disk\n";

    m_texture_cache.clear();
    std::cout << "Cleared all textures from memory\n";

    delete m_shader_program;
    std::cout << "Marked shader program for deconstruction.\n";

    SDL_GL_DeleteContext(m_opengl_context);
    SDL_DestroyWindow(m_window_handle);
    SDL_Quit();
    std::cout << "Destroyed sdl runtime.\n";

    std::cout << "DEBUG OUTPUT OF SETTINGS:\n" << m_settings.full_settings_report("    ");

    std::cout << "----- Finished unloading engine, quitting... -----" << std::endl;
}