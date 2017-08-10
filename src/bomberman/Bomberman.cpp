#include "Bomberman.hpp"

//////////////////////////////////////////////////////////////////////////
//                               Gameloop                               //
//////////////////////////////////////////////////////////////////////////

void Bomberman::game_loop()
{
    std::cout << "----- Entering gameloop -----\n";
    m_current_engine_tick = 0;
    double tick_duration = 1000.0 / m_tick_rate;

    auto current_time = std::chrono::high_resolution_clock::now();
    auto previous_time = current_time;
    double lag = 0.0;

    double in_this_second = 0.0;
    size_t updates = 0;
    size_t render_passes = 0;

    m_default_font = new engine::Font("textures/font");
    engine::RenderableString *fps_counter = m_default_font->renderable_string(utility::to_uppercase("fps/lps: ?/?"));
    {
        math::Matrix4x4<float> mat;
        mat = mat.translate((float)1, m_settings.as_int("__scr_h") - 17.0f, (float)0);

        fps_counter->transform(mat);
        fps_counter->color(math::Vector4<float>(0.0, 1.0, 0.0, 1.0));
    } //mat gets removed from stack.
    std::cout << "Finished setting up font and fps-counter...\n";

    bool running = true;
    while(running)
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

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYUP:
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                        running = false;
                    break;
            }
        }

        while(lag >= tick_duration)
        {
            //logic update

            lag -= tick_duration;
            updates++;
            m_current_engine_tick++;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        //draw game

        //draw fps counter
        fps_counter->draw();

        //swap context
        SDL_GL_SwapWindow(m_window_handle);
        render_passes++;
    }
    std::cout << "\n ----- Exited gameloop -----\n" << std::endl;
}
