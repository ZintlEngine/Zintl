#include <iostream>
#include <stdexcept>
#include <zintl/framework/app.h>

using namespace zintl;

#ifdef _WIN32
#include <Windows.h>
//extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//extern "C" __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif

int main() {
    framework::RealtimeApp app;
    try { app.run(); } catch (const std::runtime_error &err) {
        std::cout << err.what() << std::endl;
    }

    return 0;
}
