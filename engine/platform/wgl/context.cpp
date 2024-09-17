#include <stdexcept>
#include <zintl/platform/wgl.h>
#include <Windows.h>

namespace zintl::platform {
    WGLContext::WGLContext() {
        this->dc = nullptr;
        this->rc = nullptr;
    }

    LRESULT wGLDummyWndproc(
      HWND hwnd,
      UINT message,
      WPARAM wParam,
      LPARAM lParam
    ) {
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }

    std::tuple<PFNWGLCHOOSEPIXELFORMATARBPROC, PFNWGLCREATECONTEXTATTRIBSARBPROC> WGLContext::getWGLExtensions() {
        // Create dummy window to get WGL extensions
        const auto className = L"wglDummy";
        const auto hInstance = GetModuleHandle(nullptr);

        WNDCLASSEXW wndClass;
        ZeroMemory(&wndClass, sizeof(wndClass));
        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wndClass.lpszClassName = className;
        wndClass.hInstance = hInstance;
        wndClass.lpfnWndProc = wGLDummyWndproc;

        const auto dummyHwnd = CreateWindowExW(
            0,
            className,
            L"",
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            0, 0, 1, 1,
            nullptr, nullptr, hInstance, nullptr);

        const auto dummyDC = GetDC(dummyHwnd);

        PIXELFORMATDESCRIPTOR dummyPFD;
        ZeroMemory(&dummyPFD, sizeof(dummyPFD));
        dummyPFD.nSize = sizeof(dummyPFD);
        dummyPFD.nVersion = 1;
        dummyPFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        dummyPFD.iPixelType = PFD_TYPE_RGBA;
        dummyPFD.cColorBits = 32;
        dummyPFD.cAlphaBits = 8;
        dummyPFD.cDepthBits = 24;

        const auto pixelFormat = ChoosePixelFormat(dummyDC, &dummyPFD);
        if (!pixelFormat) throw std::runtime_error("ChoosePixelFormat failed");
        if (!SetPixelFormat(dummyDC, pixelFormat, &dummyPFD)) throw std::runtime_error("Failed to set Pixel format");

        const auto dummyContext = wglCreateContext(dummyDC);
        if (!dummyContext) throw std::runtime_error("Cannot create context");

        if (!wglMakeCurrent(dummyDC, dummyContext)) throw std::runtime_error("wglMakeCurrent failed");

        const auto wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(
            wglGetProcAddress("wglChoosePixelFormatARB"));
        if (!wglChoosePixelFormatARB) throw std::runtime_error("wglGetProcAddress failed");

        const auto wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(
            wglGetProcAddress("wglCreateContextAttribsARB"));
        if (!wglCreateContextAttribsARB) throw std::runtime_error("wglGetProcAddress failed");

        // Destroy props
        wglMakeCurrent(dummyDC, nullptr);
        wglDeleteContext(dummyContext);
        ReleaseDC(dummyHwnd, dummyDC);
        DestroyWindow(dummyHwnd);

        return {wglChoosePixelFormatARB, wglCreateContextAttribsARB};
    }


    void WGLContext::init(PlatformWindow &window) {
        std::tuple<PFNWGLCHOOSEPIXELFORMATARBPROC, PFNWGLCREATECONTEXTATTRIBSARBPROC> extensions = getWGLExtensions();
        const auto wglChoosePixelFormatARB = std::get<0>(extensions);
        const auto wglCreateContextAttribsARB = std::get<1>(extensions);

        const auto hwnd = static_cast<HWND>(window.rawWindowHandle());
        this->dc = GetDC(hwnd);
        if (!this->dc) throw std::runtime_error("Cannot get window DC");

        const int pixelAttribs[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB, 4,
            0
        };

        int pixelFormatId;
        UINT numFormats;
        const auto res = wglChoosePixelFormatARB(this->dc, pixelAttribs, nullptr, 1, &pixelFormatId, &numFormats);
        if (!res) throw std::runtime_error("wglChoosePixelFormatARB failed");
        if (numFormats == 0) throw std::runtime_error("No formats available");

        PIXELFORMATDESCRIPTOR pFD;
        DescribePixelFormat(this->dc, pixelFormatId, sizeof(pFD), &pFD);
        SetPixelFormat(this->dc, pixelFormatId, &pFD);

        constexpr int major = 4, minor = 6;
        constexpr int contextAttribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, major,
            WGL_CONTEXT_MINOR_VERSION_ARB, minor,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        this->rc = wglCreateContextAttribsARB(this->dc, nullptr, contextAttribs);
        if (!this->rc) throw std::runtime_error("wglCreateContextAttribsARB failed");
    }

    void WGLContext::makeCurrent() {
        if (!wglMakeCurrent(this->dc, this->rc)) throw std::runtime_error("wglMakeCurrent failed");
    }

    void WGLContext::swapBuffers() {
        SwapBuffers(this->dc);
    }
}
