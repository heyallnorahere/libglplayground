#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE ecs_example
#include <libglplayground.h>
#ifdef SYSTEM_MACOSX
constexpr int32_t major_opengl_version = 3;
#else
constexpr int32_t major_opengl_version = 4;
#endif
using namespace libplayground::gl;
namespace ecs_example {
    class ecs_example_app : public application {
    public:
        ecs_example_app() : application("ECS example", 800, 600, major_opengl_version) { }
    protected:
        virtual void load_content() override {
            // todo: load textures, shaders, and objects
        }
        virtual void render() override {
            // todo: set the renderer's shader and calculate matricies
        }
    };
    ref<application> get_application_instance() {
        return ref<ecs_example_app>::create();
    }
}