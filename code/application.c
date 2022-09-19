#include <windows.h>

int WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_flag)
{
    HMODULE library = LoadLibrary("hook64.dll");
    while (!library) {
        int user_choice = MessageBox(0,
                                     "Could not load 'hook64.dll'",
                                     "Error", 
                                     MB_RETRYCANCEL|MB_ICONERROR);
        if (user_choice == IDRETRY)
        {
            library = LoadLibrary("hook64.dll");
        }
        else
        {
            return 1;
        }

    }
    return 0;
#if 0
    HHOOK SetWindowsHookExA(
        [in] int       idHook,
        [in] HOOKPROC  lpfn,
        [in] HINSTANCE hmod,
        [in] DWORD     dwThreadId
    );
#endif
}
