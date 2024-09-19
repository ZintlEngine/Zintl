#ifndef ENGINE_FRAMEWORK_APP_H
#define ENGINE_FRAMEWORK_APP_H

namespace zintl::framework {
    class App {};

    class RealtimeApp : public App {
    public:
        virtual ~RealtimeApp() = default;
        virtual void run();
    };
}

#endif
