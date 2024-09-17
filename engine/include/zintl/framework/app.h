#ifndef ENGINE_FRAMEWORK_APP_H
#define ENGINE_FRAMEWORK_APP_H

namespace zintl::framework {
    class App {};

    class RealtimeApp : public App {
    protected:
        virtual ~RealtimeApp() = default;
        void run_default();
    public:
        virtual void run();
    };
}

#endif
