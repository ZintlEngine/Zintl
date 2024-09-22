#include <iostream>
#include <stdexcept>
#include <zintl/framework/app.h>

using namespace zintl;

#include <Windows.h>

extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    framework::RealtimeApp app;
    try { app.run(); } catch (const std::runtime_error &err) {
        MessageBoxA(NULL, err.what(), "", MB_OK);
    }

    return 0;
}
