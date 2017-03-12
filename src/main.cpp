#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "extern/lodepng.h"
#include "gui/Window.hpp"

GLuint load_texture(std::string filename)
{
    GLuint rval;
    gl::GenTextures(1, &rval);

    gl::BindTexture(gl::TEXTURE_2D, rval);
    
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);

    std::cout << "texture parameters" << std::endl;
    std::cout << gui::translate_last_opengl_error() << std::endl;
    std::cout << gui::translate_last_windows_error() << std::endl;


    const char* file = filename.c_str();
    std::vector<unsigned char> image;
    unsigned int width, height;
    size_t error = lodepng::decode(image, width, height, file);

    std::cout << "decoded image" << std::endl;
    std::cout << gui::translate_last_opengl_error() << std::endl;
    std::cout << gui::translate_last_windows_error() << std::endl;


    if(error != 0)
    {
        Exception("load_texture", "lodepng failed to load texture - " + std::string(lodepng_error_text(error)));
    }

    gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, width, height, 0, gl::RGBA, gl::UNSIGNED_BYTE, image.data());
    std::cout << "glTextImage2D" << std::endl;
    std::cout << gui::translate_last_opengl_error() << std::endl;
    std::cout << gui::translate_last_windows_error() << std::endl;

    gl::BindTexture(gl::TEXTURE_2D, 0);

    return rval;
}

int main2(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    gui::Window *w = new gui::Window("hoi", true);
    std::cout << "Running " << w->get_opengl_version() << std::endl;
    GLuint texture = load_texture("textures/test_tex.png");

    bool running = true;
    gui::Event event;
    while(running)
    {
        gl::ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        gl::Clear(gl::COLOR_BUFFER_BIT);
        w->swap_buffers();

        GLenum glerr = gl::GetError();
        DWORD wderr = GetLastError();

        if(glerr != gl::NO_ERROR_) std::cout << gui::translate_opengl_error_code(glerr) << std::endl;
        if(wderr != NO_ERROR) std::cout << gui::translate_windows_error_code(wderr);

        while(w->get_event(&event))
        {
            if(event.type() == gui::BBM_WINDOW_EVENT)
            {
                switch(event.action())
                {
                    case gui::BBM_WINDOW_CLOSED:
                        delete w; running = false; break;
                    default:
                        std::cout << event << std::endl;
                }
            }
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    try 
    {
        main2(argc, argv);
    }catch(Exception &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
}