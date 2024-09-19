#include <zintl/platform/gl.h>

#include <cstdio>
#include <glad/gl.h>

namespace zintl::platform {
   void GLLoader::load(PlatformRenderContext &context) {
      int version = 0;
      ProcLoader loader = context.getProcLoader();

      // Before loading GL functions, we need to call makeCurrent.
      context.makeCurrent();

      version = gladLoadGL((GLADloadfunc)loader);
      const int major = GLAD_VERSION_MAJOR(version);
      const int minor = GLAD_VERSION_MINOR(version);
   }
}
