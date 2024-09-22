#include <iostream>
#include <sstream>
#include <zintl/platform/wgl.h>
#include <stdexcept>
#include <gl/GL.h>
#include <glad/gl.h>
#include <gl/wglext.h>
#include <Windows.h>

HMODULE library;

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
        const auto className = L"wgldummy";
        const auto hInstance = GetModuleHandle(nullptr);

        WNDCLASSEXW wndClass;
        ZeroMemory(&wndClass, sizeof(wndClass));
        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wndClass.lpszClassName = className;
        wndClass.hInstance = hInstance;
        wndClass.lpfnWndProc = wGLDummyWndproc;

        RegisterClassExW(&wndClass);

        const auto dummyHwnd = CreateWindowExW(
            0,
            className,
            L"",
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            0, 0, 1, 1,
            nullptr, nullptr, hInstance, nullptr);
        if (!dummyHwnd) {
            DWORD error = GetLastError();
            std::ostringstream stream;
            stream << "Failed to create dummy window : " << error;
            throw std::runtime_error(stream.str().c_str());
        }

        const auto dummyDC = GetDC(dummyHwnd);

        PIXELFORMATDESCRIPTOR dummyPFD = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            8,
            0,
            0, 0, 0, 0, 0,
            24,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0,
        };

        const auto pixelFormat = ChoosePixelFormat(dummyDC, &dummyPFD);
        if (pixelFormat == 0) throw std::runtime_error("ChoosePixelFormat failed");
        DescribePixelFormat(dummyDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &dummyPFD);
        if (!SetPixelFormat(dummyDC, pixelFormat, &dummyPFD)) {
            DWORD error = GetLastError();
            std::ostringstream stream;
            stream << "Failed to set Pixel format : " << error;
            throw std::runtime_error(stream.str().c_str());
        };

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

    PRCAPIProc loaderWGLGetProcAddress(const char *procName) {
        return reinterpret_cast<PRCAPIProc>(GetProcAddress(library, procName));
    }

    void WGLContext::init(PlatformWindow &window) {
        if (!library) library = LoadLibraryW(L"opengl32.dll");
        if (!library) throw std::runtime_error("Failed to load opengl32.dll");

        std::tuple<PFNWGLCHOOSEPIXELFORMATARBPROC, PFNWGLCREATECONTEXTATTRIBSARBPROC> extensions = getWGLExtensions();
        const auto wglChoosePixelFormatARB = std::get<0>(extensions);
        const auto wglCreateContextAttribsARB = std::get<1>(extensions);

        const auto hwnd = static_cast<HWND>(window.rawWindowHandle());
        this->dc = GetDC(hwnd);
        if (!this->dc) throw std::runtime_error("Cannot get window DC");

        constexpr int pixelAttribs[] = {
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
        if (!wglChoosePixelFormatARB(this->dc, pixelAttribs, nullptr, 1, &pixelFormatId, &numFormats))
            throw std::runtime_error("wglChoosePixelFormatARB failed");
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

    PRCAPIProc loaderGLGetProcAddress(const char *procName) {
        const auto proc = wglGetProcAddress(procName);
        if (!proc) {
            std::cout << "opengl: " << procName << std::endl;
            return reinterpret_cast<PRCAPIProc>(GetProcAddress(library, procName));
        }

        std::cout << "wgl: " << procName << std::endl;
        return reinterpret_cast<PRCAPIProc>(proc);
    }

    // Virtual function cannot be function pointer.
    ProcLoader WGLContext::getProcLoader() {
        return static_cast<ProcLoader>(loaderGLGetProcAddress);
    }

    void WGLContext::makeCurrent() {
        if (!wglMakeCurrent(this->dc, this->rc)) throw std::runtime_error("wglMakeCurrent failed");
    }

    void WGLContext::swapBuffers() {
        SwapBuffers(this->dc);
    }
}
