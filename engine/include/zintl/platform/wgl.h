/*
* (c) 2024-present Hasegawa Raku
 */

#ifndef ENGINE_PLATFORM_WGL_H
#define ENGINE_PLATFORM_WGL_H

#include "shared.h"

#include <tuple>

#include <gl/GL.h>
#include <gl/wglext.h>

namespace zintl::platform {
    class WGLContext final : public PlatformRenderContext {
    private:
        HDC dc;
        HGLRC rc;
        static std::tuple<PFNWGLCHOOSEPIXELFORMATARBPROC, PFNWGLCREATECONTEXTATTRIBSARBPROC> getWGLExtensions();
    public:
        WGLContext();
        void init(PlatformWindow &window) override;
        void makeCurrent() override;
        void swapBuffers() override;
    };
}

#endif
