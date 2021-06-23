#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE model_loading
#include <libglplayground.h>
#ifdef SYSTEM_MACOSX
constexpr int32_t major_opengl_version = 3;
#else
constexpr int32_t major_opengl_version = 4;
#endif
using namespace libplayground::gl;
namespace model_loading {
    class model_loading_app : public application {
    public:
        model_loading_app() : application("Model loading example", 800, 600, major_opengl_version) { }
    protected:
        virtual void load_content() override {
            this->m_animated = ref<model>::create("assets/models/animated.fbx");
        }
    private:
        ref<model> m_static, m_animated;
    };
    ref<application> get_application_instance() {
        return ref<model_loading_app>::create();
    }
}