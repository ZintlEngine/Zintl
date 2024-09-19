/*
* (c) 2024-present Hasegawa Raku
 */

#ifndef ENGINE_PLATFORM_SHARED_H
#define ENGINE_PLATFORM_SHARED_H

namespace zintl::platform {
    typedef void (*PRCAPIProc)();
    /// A type of function pointer to a process loader
    typedef PRCAPIProc (*ProcLoader)(const char *procName);

    class PlatformApplication {
    public:
        virtual ~PlatformApplication() = default;
        virtual void init() = 0;
        virtual void dispatchEvent() = 0;
    };

    /// An abstract class for platform window
    class PlatformWindow {
    public:
        virtual ~PlatformWindow() = default;
        virtual void *rawWindowHandle() = 0;
    };

    class PlatformRenderContext {
    public:
        virtual ~PlatformRenderContext() = default;
        virtual void init(PlatformWindow &window) = 0;
        virtual ProcLoader getProcLoader() = 0;
        virtual void makeCurrent() = 0;
        virtual void swapBuffers() = 0;
    };
}

#endif
