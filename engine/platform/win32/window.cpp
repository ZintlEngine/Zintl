#include <zintl/platform/win32.h>
#include <stdexcept>

#define DEFAULT_DPI 96

LRESULT wndproc(
  HWND hwnd,
  UINT message,
  WPARAM wParam,
  LPARAM lParam
) {
    return DefWindowProcW(hwnd, message, wParam, lParam);
}

namespace zintl::platform {
    HWND Win32Window::createWindow(const Win32WindowInitConfig &init) {
        const LPCWSTR className = L"ZintlWindow";

        WNDCLASSEXW wndClass;
        ZeroMemory(&wndClass, sizeof(wndClass));
        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = CS_HREDRAW | CS_VREDRAW;
        wndClass.lpszClassName = className;
        wndClass.hInstance = init.hInstance;
        wndClass.lpfnWndProc = wndproc;
        wndClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));

        RegisterClassExW(&wndClass);

        const LPCWSTR title = std::wstring(init.title.begin(), init.title.end()).c_str();

        const auto hwnd = CreateWindowExW(
            0,
            className,
            title,
            WS_OVERLAPPEDWINDOW,
            init.posX,
            init.posY,
            init.width,
            init.height,
            nullptr,
            nullptr,
            init.hInstance,
            nullptr);

        if (!hwnd) throw std::runtime_error("Failed to create window");

        const auto dpi = static_cast<int>(GetDpiForWindow(hwnd));

        if (init.hidpi) {
            SetWindowPos(
                hwnd,
                nullptr,
                0,
                0,
                init.width * dpi / DEFAULT_DPI,
                init.height * dpi / DEFAULT_DPI,
                SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);
        }

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        return hwnd;
    }

    Win32Window::Win32Window(const Win32WindowInitConfig &config) {
        this->hwnd = createWindow(config);
    }

    Win32Window::~Win32Window() {
        DestroyWindow(hwnd);
    }

    void *Win32Window::rawWindowHandle() {
        return this->hwnd;
    }
}
