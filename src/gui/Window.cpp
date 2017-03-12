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
        Window *p_this = 0; //reference to the class instance this event belongs to.
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
                    if(GetLastError() != 0) throw Exception("Window::window_proc", "WM_NCCREATE couldnt set longwindowptr - " + gui::translate_last_windows_error());
                return 1;
            /*
                WM_CREATE: initialization of window, performs opengl setup.
            */
            case WM_CREATE:
                std::cout << "WM_CREATE" << std::endl;
                std::cout << gui::translate_last_opengl_error() << std::endl;
                std::cout << gui::translate_last_windows_error() << std::endl;

                hdc = GetDC(hwnd);
                std::cout << "GetDC" << std::endl;
                std::cout << gui::translate_last_opengl_error() << std::endl;
                std::cout << gui::translate_last_windows_error() << std::endl;

                set_pixelformat_descriptor(hdc);
                std::cout << "pixelformat" << std::endl;
                std::cout << gui::translate_last_opengl_error() << std::endl;
                std::cout << gui::translate_last_windows_error() << std::endl;

                p_this->m_opengl_context = wglCreateContext(hdc);
                if(p_this->m_opengl_context == NULL)
                    throw Exception("Window::window_proc", "failed to create opengl context in WM_CREATE - " + translate_last_windows_error());
                std::cout << "wglCreateContext" << std::endl;
                std::cout << gui::translate_last_opengl_error() << std::endl;
                std::cout << gui::translate_last_windows_error() << std::endl;

                if(!wglMakeCurrent(hdc, p_this->m_opengl_context))
                    throw Exception("Window::window_proc", "failed to set opengl context as current in WM_CREATE - " + translate_last_windows_error());
                std::cout << "wglMakeCurrent" << std::endl;
                std::cout << gui::translate_last_opengl_error() << std::endl;
                std::cout << gui::translate_last_windows_error() << std::endl;
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
                Some messages are send to the callback directly, we'll post em into the queue anyway.
            */
            case WM_CLOSE:
                if(!PostMessage(p_this->m_window_handle, BBM_PASS_WM_CLOSE, w_param, l_param))
                    throw Exception("Window::window_proc", "failed to post WM_CLOSE to the messagequeue - " + translate_last_windows_error());
                return 0;
            case WM_ACTIVATEAPP:
                if(p_this->m_window_handle == 0) return 0; //because this gets posted before the handle is stored.
                if(!PostMessage(p_this->m_window_handle, BBM_PASS_WM_ACTIVATEAPP, w_param, l_param))
                    throw Exception("Window::window_proc", "failed to post WM_ACTIVATEAPP to the messagequeue - " + translate_last_windows_error());
                return 0;
            
            case WM_WINDOWPOSCHANGED: //TODO: this message gets posted to dispatch not peekmessage.
                //should rework this to not beeing resizable from user side.
                p_this->m_window_pos.x(((WINDOWPOS*)l_param)->x);
                p_this->m_window_pos.y(((WINDOWPOS*)l_param)->y);
                p_this->m_window_pos.width(((WINDOWPOS*)l_param)->cx);
                p_this->m_window_pos.height(((WINDOWPOS*)l_param)->cy);

                if(!PostMessage(p_this->m_window_handle, BBM_PASS_WM_MOVE, 0, 0))
                    throw Exception("Window::window_proc", "failed to post BBM_PASS_WM_MOVE to the messagequeue - " + translate_last_windows_error());
                return 0;

            /*
                When we receive a message we dont care about, make sure its handeled properly
            */
            default:
                //std::cout << "Sending DefWindowProc: " << translate(msg, T_MESSAGE) << std::endl;
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

    Window::Window(const std::string &title, bool border, size_t x, size_t y, size_t w, size_t h)
    {
        //get running instance handle
        HINSTANCE h_instance = GetModuleHandle(NULL);
        m_window_handle = 0;

        //create and register window class
        //TODO: What happens on 2nd instance?
        WNDCLASSEX wc;
        const char classname[] = "opengl_3_3_window_class";
        memset(&wc, 0, sizeof(WNDCLASSEX));
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = &Window::window_proc;
        wc.hInstance = h_instance;
        wc.lpszClassName = classname;
        
        if(!RegisterClassEx(&wc))
            throw Exception("Window::Window", "failed to register window class");

        //create the window
        m_window_handle = CreateWindowEx
                                (
                                    WS_EX_APPWINDOW,
                                    classname,
                                    title.c_str(),
                                    (border ? WS_OVERLAPPEDWINDOW : WS_POPUP) | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
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
    }

    Window::~Window()
    {
        DestroyWindow(m_window_handle);
        wglDeleteContext(m_opengl_context);
    }


    bool Window::get_event(Event *event)
    {
        MSG msg;

        if(!PeekMessage(&msg, m_window_handle, 0, 0, PM_REMOVE))
        {
            //there was no message
            return false;
        }

        bool dragged;
        int dx, dy, button;

        switch(msg.message)
        {
            /*
                Window events
            */
            case BBM_PASS_WM_ACTIVATEAPP:
                *event = Event(BBM_WINDOW_EVENT, msg.wParam ? BBM_WINDOW_GAINEDFOCUS : BBM_WINDOW_LOSTFOCUS, msg.time);
                return true;
            case BBM_PASS_WM_MOVE:
                std::cout << "received bbmpasswmmove" << std::endl;
                *event = Event(BBM_WINDOW_EVENT, BBM_WINDOW_MOVED, msg.time);
                return true;
            case BBM_PASS_WM_CLOSE:
                *event = Event(BBM_WINDOW_EVENT, BBM_WINDOW_CLOSED, msg.time);
                return true;

            /*
                Keyboard events
            */
            case WM_KEYDOWN:
                *event = Event(BBM_KEYBOARD_EVENT, BBM_KEYDOWN, msg.time, msg.wParam);
                return true;
            case WM_KEYUP:
                *event = Event(BBM_KEYBOARD_EVENT, BBM_KEYUP, msg.time, msg.wParam);
                return true;
            
            /*
                Mousemotion
            */
            case WM_MOUSEMOVE:
                dx = m_mouse_pos.x() - GET_X_LPARAM(msg.lParam);
                dy = m_mouse_pos.y() - GET_Y_LPARAM(msg.lParam);
                m_mouse_pos.x(GET_X_LPARAM(msg.lParam));
                m_mouse_pos.y(GET_Y_LPARAM(msg.lParam));
                dragged = msg.wParam & MK_LBUTTON;

                *event = Event(BBM_MOUSEMOTION_EVENT, dragged ? BBM_MOUSEDRAGGED : BBM_MOUSEMOVED, msg.time, 0, m_mouse_pos.x(), m_mouse_pos.y(), dx, dy);
                return true;

            /*
                Mousebuttons clicked
            */
            case WM_LBUTTONDOWN:
                dx = GET_X_LPARAM(msg.lParam);
                dy = GET_Y_LPARAM(msg.lParam);

                *event = Event(BBM_MOUSE_EVENT, BBM_MOUSEBUTTONDOWN, msg.time, VK_LBUTTON, dx, dy);
                return true;
            case WM_LBUTTONUP:
                dx = GET_X_LPARAM(msg.lParam);
                dy = GET_Y_LPARAM(msg.lParam);

                *event = Event(BBM_MOUSE_EVENT, BBM_MOUSEBUTTONUP, msg.time, VK_LBUTTON, dx, dy);
                return true;
            
            case WM_MBUTTONDOWN:
                dx = GET_X_LPARAM(msg.lParam);
                dy = GET_Y_LPARAM(msg.lParam);

                *event = Event(BBM_MOUSE_EVENT, BBM_MOUSEBUTTONDOWN, msg.time, VK_MBUTTON, dx, dy);
                return true;
            case WM_MBUTTONUP:
                dx = GET_X_LPARAM(msg.lParam);
                dy = GET_Y_LPARAM(msg.lParam);

                *event = Event(BBM_MOUSE_EVENT, BBM_MOUSEBUTTONUP, msg.time, VK_MBUTTON, dx, dy);
                return true;

            case WM_RBUTTONDOWN:
                dx = GET_X_LPARAM(msg.lParam);
                dy = GET_Y_LPARAM(msg.lParam);

                *event = Event(BBM_MOUSE_EVENT, BBM_MOUSEBUTTONDOWN, msg.time, VK_RBUTTON, dx, dy);
                return true;
            case WM_RBUTTONUP:
                dx = GET_X_LPARAM(msg.lParam);
                dy = GET_Y_LPARAM(msg.lParam);

                *event = Event(BBM_MOUSE_EVENT, BBM_MOUSEBUTTONUP, msg.time, VK_RBUTTON, dx, dy);
                return true;
            
            case WM_XBUTTONDOWN:
                button = GET_XBUTTON_WPARAM(msg.wParam);
                dx = GET_X_LPARAM(msg.lParam);
                dy = GET_Y_LPARAM(msg.lParam);

                *event = Event(BBM_MOUSE_EVENT, BBM_MOUSEBUTTONDOWN, msg.time, button, dx, dy);
                return true;

            case WM_XBUTTONUP:
                button = GET_XBUTTON_WPARAM(msg.wParam);
                dx = GET_X_LPARAM(msg.lParam);
                dy = GET_Y_LPARAM(msg.lParam);

                *event = Event(BBM_MOUSE_EVENT, BBM_MOUSEBUTTONUP, msg.time, button == 1 ? VK_XBUTTON1 : VK_XBUTTON2, dx, dy);
                return true;
            //We dont want to report these
            case WM_NCMOUSEMOVE:
                DispatchMessage(&msg);
                return false;
            //If we dont specifically handle this message
            default:
                //std::cout << "unencoded message: " << translate(msg.message, T_MESSAGE) << std::endl;
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
        return std::string(reinterpret_cast<const char*>(gl::GetString(gl::VERSION)));
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

    std::string translate_last_opengl_error()
    {
        return translate_opengl_error_code(gl::GetError());
    }

    std::string translate_opengl_error_code(GLenum error)
    {
        switch(error)
        {
            case gl::NO_ERROR_: return "GL_NO_ERROR";
            case gl::INVALID_ENUM: return "GL_INVALID_ENUM";
            case gl::INVALID_VALUE: return "GL_INVALID_VALUE";
            case gl::INVALID_OPERATION: return "GL_INVALID_OPERATION";
            case gl::OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
            default: return "UNKNOWN_ERROR";
        }
    }

}