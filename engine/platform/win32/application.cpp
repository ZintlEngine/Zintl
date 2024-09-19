#include <zintl/platform/win32.h>

#include <winuser.h>

namespace zintl::platform {
    Win32Application::~Win32Application() {

    }

    void Win32Application::enableHidpiSupport() {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    }

    void Win32Application::init() {
        enableHidpiSupport();
    }

    void Win32Application::dispatchEvent() {
        MSG msg;

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                exit(0);
            }
        }
    }

}
