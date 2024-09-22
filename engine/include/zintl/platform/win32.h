/*
* (c) 2024-present Hasegawa Raku
 */

#ifndef ENGINE_PLATFORM_WIN32_H
#define ENGINE_PLATFORM_WIN32_H

#include <string>
#include <Windows.h>

#include "shared.h"

namespace zintl::platform {
    class Win32Application final : public PlatformApplication {
    private:
        static void enableHidpiSupport();
    public:
        void init() override;
        void dispatchEvent() override;

        explicit Win32Application() = default;
        ~Win32Application() override;
    };

    /// Setting a position of -1 means to use the system specified value.
    struct Win32WindowInitConfig {
        HINSTANCE hInstance;
        int posX = -1, posY = -1, width = -1, height = -1;
        std::string title{};
        bool hidpi = true;
        bool darkMode = false;
    };

    class Win32Window final : public PlatformWindow {
    private:
        HWND hwnd;
        static HWND createWindow(const Win32WindowInitConfig &config);
        static void applyDarkMode(const HWND& hwnd);

    public:
        void *rawWindowHandle() override;

        explicit Win32Window(const Win32WindowInitConfig &config);
        ~Win32Window() override;
    };
}

#endif
