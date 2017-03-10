#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "gui/Window.hpp"

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    gui::Window *w = new gui::Window("hoi");
    
    gl::ClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);
    w->swap_buffers();

    bool running = true;
    gui::Event event;
    while(running)
    {
        gl::ClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);
        w->swap_buffers();

        while(w->get_event(&event))
        {
            std::cout << event << std::endl;
        }
    }

    return 0;
}