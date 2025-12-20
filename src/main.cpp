#include "App.hpp"

int main()
{
    // const wchar_t* windowTitle = L"Dungeon Defenders 2";

    // HWND hWnd = FindWindowW(NULL, windowTitle);
    // if (hWnd == NULL)
    // {
    //     std::wcout << L"Window not found" << std::endl;
    //     return 1;
    // }

    // WPARAM keyDown = 0x41; 
    // WPARAM keyUp = 0x41;

    // PostMessageW(hWnd, WM_KEYDOWN, keyDown, 0);
    // Sleep(50); 
    // PostMessageW(hWnd, WM_KEYUP, keyUp, 0);
    App app;
    app.Run();

    return 0;
}