/*
* (c) 2024-present Hasegawa Raku
 */

#ifndef ENGINE_PLATFORM_WGL_H
#define ENGINE_PLATFORM_WGL_H

#if _WIN32

#include "shared.h"

#include <tuple>

#include <glad/gl.h>
#include <gl/wglext.h>

namespace zintl::platform {
    class WGLContext;

    class WGLContext final : public PlatformRenderContext {
    private:
        HDC dc;
        HGLRC rc;
        static std::tuple<PFNWGLCHOOSEPIXELFORMATARBPROC, PFNWGLCREATECONTEXTATTRIBSARBPROC> getWGLExtensions();
    public:
        WGLContext();
        void init(PlatformWindow &window) override;
        ProcLoader getProcLoader() override;
        void makeCurrent() override;
        void swapBuffers() override;
    };
}

#endif

#endif
