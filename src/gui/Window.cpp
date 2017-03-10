#include "Window.hpp"

//tijdelijk
#include <iostream>
#include "../tools/WinCodeTranslate.hpp"

namespace gui
{

    /*
        Static functions
    */

    LRESULT CALLBACK Window::window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
    {
        Window *p_this; //reference to the class instance this event belongs to.
        HDC hdc; //current device context

        if(msg != WM_NCCREATE) //if this is not first create call
        {   
            //retrieve class instance pointer
            p_this = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        switch(msg)
        {
            /*
                WM_NCCREATE: pre-initialization, stores object pointer with window instance
            */
            case WM_NCCREATE:
                p_this = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(l_param)->lpCreateParams);

                SetLastError(0); //ensure error is reset
                if(!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_this)))
                    if(GetLastError() != 0) return 0;
                return 1;
            /*
                WM_CREATE: initialization of window, performs opengl setup.
            */
            case WM_CREATE:
                hdc = GetDC(hwnd);
                set_pixelformat_descriptor(hdc);

                p_this->m_opengl_context = wglCreateContext(hdc);
                if(p_this->m_opengl_context == NULL)
                    throw Exception("Window::window_proc", "failed to create opengl context in WM_CREATE - " + translate_last_windows_error());
                
                if(!wglMakeCurrent(hdc, p_this->m_opengl_context))
                    throw Exception("Window::window_proc", "failed to set opengl context as current in WM_CREATE - " + translate_last_windows_error());

                return 0; //all went well
            /*
                WM_PAINT: when this message is requested the surface gets revalidated to supress further events.
            */
            case WM_PAINT:
                BeginPaint(hwnd, &(p_this->m_ps));
                EndPaint(hwnd, &(p_this->m_ps));
                return 0; //all went well
            /*
                WM_SETCURSOR: Reset cursor to default cursor when requested
            */
            case WM_SETCURSOR:
                SetCursor(p_this->m_cursor);
                return 1; //all went well
            
            /*
                When we receive a message we dont care about, make sure its handeled properly
            */
            default:
                return DefWindowProc(hwnd, msg, w_param, l_param);
        }
    }

    void Window::set_pixelformat_descriptor(HDC hdc)
    {
        PIXELFORMATDESCRIPTOR pfd = 
        {
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //flags
            PFD_TYPE_RGBA,          //framebuffertype
            32,                     //colordepth of the framebuffer
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                        //Number of bits for the depthbuffer
            8,                        //Number of bits for the stencilbuffer
            0,                        //Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        int format = ChoosePixelFormat(hdc, &pfd); //requests most appropriate format
        if(format == 0)
            throw Exception("Window::set_pixelformat_descriptor", "failed to find an appropriate pixelformat descriptor - " + translate_last_windows_error());
        SetPixelFormat(hdc, format, &pfd);
    }

    /*
        Non-static member functions
    */

    Window::Window(const std::string &title, size_t x, size_t y, size_t w, size_t h)
    {
        //get running instance handle
        HINSTANCE h_instance = GetModuleHandle(NULL);

        //create and register window class
        //TODO: What happens on 2nd instance?
        WNDCLASSEX wc;
        const char classname[] = "opengl_3_3_window_class";
        memset(&wc, 0, sizeof(WNDCLASSEX));
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = &Window::window_proc;
        wc.hInstance = h_instance;
        wc.lpszClassName = classname;
        RegisterClassEx(&wc);

        //create the window
        m_window_handle = CreateWindowEx
                                (
                                    WS_EX_APPWINDOW,
                                    classname,
                                    title.c_str(),
                                    WS_OVERLAPPEDWINDOW,
                                    x, y, w, h,
                                    NULL, NULL,
                                    h_instance,
                                    this
                                );
        
        if(m_window_handle == NULL)
            throw Exception("Window::Window", "error creating window - " + translate_last_windows_error());
        
        ShowWindow(m_window_handle, 5);
        m_hdc = GetDC(m_window_handle);
        m_cursor = LoadCursor(NULL, IDC_ARROW);
        m_mouse_x = m_mouse_y = 0;
    }

    bool Window::get_event(Event *event)
    {
        MSG msg;

        if(!PeekMessage(&msg, m_window_handle, 0, 0, PM_REMOVE))
        {
            return false;
        }

        std::cout << "message: " << translate(msg.message, T_MESSAGE) << std::endl;

        bool dragged;
        int dx, dy;

        switch(msg.message)
        {
            case WM_KEYDOWN:
                *event = Event(BBM_KEYBOARD_EVENT, BBM_KEYDOWN, msg.time, msg.wParam);
                return true;
            case WM_KEYUP:
                *event = Event(BBM_KEYBOARD_EVENT, BBM_KEYUP, msg.time, msg.wParam);
                return true;
            case WM_MOUSEMOVE:
                dx = m_mouse_x - GET_X_LPARAM(msg.lParam);
                dy = m_mouse_y - GET_Y_LPARAM(msg.lParam);
                m_mouse_x = GET_X_LPARAM(msg.lParam);
                m_mouse_y = GET_Y_LPARAM(msg.lParam);
                dragged = msg.wParam & MK_LBUTTON;

                *event = Event(BBM_MOUSEMOTION_EVENT, dragged ? BBM_MOUSEDRAGGED : BBM_MOUSEMOVED, msg.time, 0, m_mouse_x, m_mouse_y, dx, dy);
                return true;
            //If we dont specifically handle this message
            default:
                DispatchMessage(&msg);
                return false;
        }
    }

    void Window::swap_buffers() const
    {
        SwapBuffers(m_hdc);
    }

    std::string Window::get_opengl_version() const
    {
        return "NOT IMPLEMENTED YET";
    }

    std::string Window::to_string() const
    {
        return "Opengl window";
    }

    /*
        non-member functions
    */

    std::string translate_last_windows_error()
    {
        std::string msg = translate_windows_error_code(GetLastError());
        SetLastError(0);
        return msg;
    }

    std::string translate_windows_error_code(DWORD error)
    {
        LPVOID msg;

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&msg,
            0, NULL);

        return std::string(static_cast<char*>(msg));
    }

}