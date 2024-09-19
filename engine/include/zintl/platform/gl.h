#ifndef ENGINE_PLATFORM_GL_H
#define ENGINE_PLATFORM_GL_H

#include <glad/gl.h>

#include "shared.h"

namespace zintl::platform {
    class GLLoader {
    public:
        static void load(PlatformRenderContext &context);
    };
}
#endif
