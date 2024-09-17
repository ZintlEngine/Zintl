/*
* (c) 2024-present Hasegawa Raku
 */

#ifndef ENGINE_PLATFORM_SHARED_H
#define ENGINE_PLATFORM_SHARED_H

namespace zintl::platform {
    class PlatformApplication {
    public:
        virtual ~PlatformApplication() = default;
        virtual void init();
        virtual void dispatchEvent();
    };

    /// An abstract class for platform window
    class PlatformWindow {
    public:
        virtual ~PlatformWindow() = default;
        virtual void *rawWindowHandle();
    };

    class PlatformRenderContext {
    public:
        virtual ~PlatformRenderContext() = default;
        virtual void init(PlatformWindow &window);
        virtual void makeCurrent();
        virtual void swapBuffers();
    };
}

#endif
