#include <iostream>
#include <zintl/framework/app.h>
#include <zintl/platform/gl.h>
#include <zintl/platform/wgl.h>
#include <zintl/platform/win32.h>

namespace zintl::framework {
    void RealtimeApp::run() {
        platform::Win32WindowInitConfig config;
        config.posX = 0;
        config.posY = 0;
        config.width = 800;
        config.height = 600;
        config.title = "window";

        platform::Win32Application app;
        app.init();

        platform::Win32Window window(config);
        platform::WGLContext context;
        context.init(window);

        platform::GLLoader::load(context);

        std::cout << "Init" << std::endl;

        context.makeCurrent();

        std::cout << "makeCurrent" << std::endl;

        while (true) {
            app.dispatchEvent();

            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            context.swapBuffers();
        }
    }

}
