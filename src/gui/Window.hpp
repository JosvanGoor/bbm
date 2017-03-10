#ifndef BBM_GUI_WINDOW_HPP
#define BBM_GUI_WINDOW_HPP

#include <string>
#include <windows.h>
#include <windowsx.h>

#include "Event.hpp"
#include "../Core.hpp"
#include "../extern/gl_core_3_3.hpp"

namespace gui
{

    class Window : public Object
    {
    protected:
        HDC m_hdc; //run-instance handle
        PAINTSTRUCT m_ps; //PAINTSTRUCT
        HCURSOR m_cursor; //default cursor handle
        HWND m_window_handle; //windowhandle
        HGLRC m_opengl_context; //opengl context handle

        //latest observed mouse coordinates 
        int m_mouse_x;
        int m_mouse_y;

        static void set_pixelformat_descriptor(HDC hdc); //used in setup
        //callback function
        static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

        //virtual override
        virtual std::string to_string() const;

    public:
        Window(const std::string &title, size_t x = 200, size_t y = 200, size_t w = 1366, size_t h = 768);
        ~Window();

        bool get_event(Event *event);
        void swap_buffers() const;
        std::string get_opengl_version() const;
    };

    //translate windows error code
    std::string translate_last_windows_error(); //resets error code
    std::string translate_windows_error_code(DWORD error);

}

#endif
