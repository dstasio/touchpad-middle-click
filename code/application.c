#include <windows.h>

static int global_running;

LRESULT CALLBACK
window_proc(HWND window, UINT message, WPARAM w, LPARAM l)
{
    LRESULT result = 0;
    switch(message)
    {
        case WM_CLOSE:
        {
            DestroyWindow(window);
        } break;
            
        case WM_DESTROY:
        {
            global_running = 0;
            PostQuitMessage(0);
        } break;

        // @todo: implement this
        case WM_SETCURSOR:
        {
        } break;

        default:
        {
          result = DefWindowProcA(window, message, w, l);
        } break;
    }

    return result;
}

int WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_flag)
{
    // hook
    HMODULE library = LoadLibrary(L"hook64.dll");
    while (!library) {
        int user_choice = MessageBox(0,
                                     L"Could not load 'hook64.dll'",
                                     L"Error", 
                                     MB_RETRYCANCEL|MB_ICONERROR);
        if (user_choice == IDRETRY)
            library = LoadLibrary(L"hook64.dll");
        else
            return 1;
    }

    HOOKPROC hook_proc = (HOOKPROC)GetProcAddress(library, "hook_proc");
    if (!hook_proc)
        return 1;

    HHOOK hook = SetWindowsHookExA(WH_GETMESSAGE,
                                   hook_proc,
                                   library,
                                   0);


    // window
    WNDCLASSA wnd_class       = {0};
    wnd_class.style           = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    wnd_class.lpfnWndProc     = window_proc;
    //wnd_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wnd_class.hInstance       = instance;
    wnd_class.lpszClassName   = "wnd_class";
    RegisterClassA(&wnd_class);

    HWND window = CreateWindowA("wnd_class", "Title",
                                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                300,
                                200,
                                0, 0, instance, 0);

    if(window) {
        global_running = 1;
        MSG message = {0};

        int mbutton = 0;

        while(global_running)
        {
            mbutton = 0;

            while(PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
            {
                switch(message.message)
                {
                    case WM_KEYDOWN:
                    {
                        if (message.wParam == VK_ESCAPE) global_running = 0;
                    } break;

                    case WM_MBUTTONDOWN:
                        mbutton = 1;
                        break;

                    default:
                    {
                        TranslateMessage(&message);
                        DispatchMessage (&message);
                    } break;
                }
            }

            if (mbutton) {
                global_running = 0;
            }
        }
    }


    // hook
    UnhookWindowsHookEx(hook);

    return 0;
}
