#include "Bomberman.hpp"

#include "menus/MainMenu.hpp"

//////////////////////////////////////////////////////////////////////////
//                               Gameloop                               //
//////////////////////////////////////////////////////////////////////////

void Bomberman::game_loop()
{
    std::cout << "----- Entering gameloop -----\n";
    m_current_engine_tick = 0;
    float tick_duration = 1000.0 / m_tick_rate;

    std::cout << "Setting up timers...";
    auto current_time = std::chrono::high_resolution_clock::now();
    auto previous_time = current_time;
    float lag = 0.0;

    float in_this_second = 0.0;
    size_t updates = 0;
    size_t render_passes = 0;
    std::cout << " done!\n";

    std::cout << "Setting up font and fps-counter...";
    m_default_font = new engine::Font("textures/font");
    engine::RenderableString *fps_counter = m_default_font->renderable_string(utility::to_uppercase("fps/lps: ?/?"));
    {
        math::Matrix4x4<float> mat;
        mat = mat.translate((float)1, m_settings.as_int("__scr_h") - 17.0f, (float)0);

        fps_counter->transform(mat);
        fps_counter->color(math::Vector4<float>(0.0, 1.0, 0.0, 0.75));
    } //mat gets removed from stack.
    std::cout << " done!\n";

    m_game_state = new MainMenu();
    std::cout << "Created initial engine::GameStateController\n";

    m_game_running = true;
    while(m_game_running)
    {
        current_time = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(current_time - previous_time).count();
        lag += elapsed;
        previous_time = current_time;

        in_this_second += elapsed;
        if(in_this_second >= 1000.0)
        {
            std::cout << "\rfps/logic - " << render_passes << " / " << updates << std::flush;
            m_default_font->renderable_string(fps_counter, utility::to_uppercase("fps/lps: " + std::to_string(render_passes) + "/" + std::to_string(updates)));

            updates = render_passes = 0;
            in_this_second -= 1000.0;
        }

        handle_events();

        while(lag >= tick_duration)
        {
            //logic update
            engine::GameStateController *rval = m_game_state->logic_update();
            m_mouse.tick();

            if(rval != nullptr)
            {
                delete m_game_state;
                m_game_state = rval;
                m_current_engine_tick = 1; //TODO: ?
            }

            lag -= tick_duration;
            updates++;
            m_current_engine_tick++;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        //draw game
        m_game_state->draw_to_screen(lag / tick_duration);

        //draw fps counter
        math::Matrix4f temp = m_default_view;
        m_default_view = math::Matrix4f(); //TODO: optimize pls.
        fps_counter->draw();
        m_default_view = temp;

        //swap context
        SDL_GL_SwapWindow(m_window_handle);
        render_passes++;
    }

    delete m_game_state;
    std::cout << "\nDestroyed current engine::GameStateController.\n";

    std::cout << "----- Exited gameloop -----\n" << std::endl;
}

void Bomberman::handle_events()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                quit();
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    quit();
            case SDL_KEYUP:
                m_keyboard->update(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
                m_mouse.update(event);
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
                    m_gamepad_1 = new engine::GamepadController(SDL_GameControllerOpen(event.cdevice.which));
                    m_gamepad_1->simple_rumble(1.0, 750);
                    std::cout << "\rNew controller mapped to location 1...\n";
                }
                else if(!m_gamepad_2->connected()) 
                {
                    m_gamepad_2 = new engine::GamepadController(SDL_GameControllerOpen(event.cdevice.which));
                    m_gamepad_2->simple_rumble(1.0, 750);
                    std::cout << "\rNew controller mapped to location 2...\n";
                }
                else if(!m_gamepad_3->connected()) 
                {
                    m_gamepad_3 = new engine::GamepadController(SDL_GameControllerOpen(event.cdevice.which));
                    m_gamepad_3->simple_rumble(1.0, 750);
                    std::cout << "\rNew controller mapped to location 3...\n";
                }
                else if(!m_gamepad_4->connected()) 
                {
                    m_gamepad_4 = new engine::GamepadController(SDL_GameControllerOpen(event.cdevice.which));
                    m_gamepad_4->simple_rumble(1.0, 750);
                    std::cout << "\rNew controller mapped to location 4...\n";
                }
        }
    }
}
