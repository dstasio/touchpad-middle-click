#include <windows.h>

LRESULT CALLBACK hook_proc(int    code,
                           WPARAM w,
                           LPARAM l)
{
    if (code < 0)
    {
        return CallNextHookEx(0, code, w, l);
    }

    MSG *message = (MSG*)l;

    switch (message->message) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        {
	    SHORT lwin = GetKeyState(VK_LWIN);
	    SHORT rwin = GetKeyState(VK_RWIN);
            SHORT ctrl = GetKeyState(VK_LCONTROL);

            // high order bit: key is pressed
            //  low order bit: key is toggled
	    //if (((lwin >> 7) & 1) || ((rwin >> 7) & 1))
            if ((ctrl >> 7) & 1)
            {
	        message->message += WM_MBUTTONDOWN - WM_LBUTTONDOWN;
            }
        } break;

        default:
        break;
    }

    return CallNextHookEx(0, code, w, l);
}



#if 0
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203

#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
#endif
