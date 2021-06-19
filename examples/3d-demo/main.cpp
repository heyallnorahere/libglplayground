#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE _3d_demo
#include <libglplayground.h>
#ifdef SYSTEM_MACOSX
constexpr int32_t major_opengl_version = 3;
#else
constexpr int32_t major_opengl_version = 4;
#endif
using namespace libplayground::gl;
namespace _3d_demo {
    struct vertex {
        glm::vec3 pos, color;
    };
    class _3d_demo_app : public application {
    public:
        _3d_demo_app() : application("3d-demo", 800, 600, major_opengl_version) { }
    private:
        virtual void load_content() override {
            // todo: load content
        }
        virtual void update() override {
            // todo: update matricies
        }
        virtual void render() override {
            // todo: send matricies to shader, and render
        }
    };
    ref<application> get_application_instance() {
        return ref<_3d_demo_app>::create();
    }
}