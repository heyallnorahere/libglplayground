#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE model_loading
#include <libglplayground.h>
#if defined(SYSTEM_MACOSX) && !defined(NDEBUG)
constexpr bool mesa_context = true;
#else
constexpr bool mesa_context = false;
#endif
using namespace libplayground::gl;
namespace model_loading {
    class model_loading_app : public application {
    public:
        model_loading_app() : application("Model loading example", 800, 600, mesa_context, 4) { }
    protected:
        virtual void load_content() override {
            shader_factory factory;
            auto& library = shader_library::get();
            library["model-animated"] = factory.single_file("assets/shaders/model-loading-animated.glsl");
            entity e = this->m_scene->create();
            e.add_component<components::model_component>(ref<model>::create("assets/models/animated.fbx"), 0);
            this->m_camera = this->m_scene->create();
            auto& transform = this->m_camera.get_component<components::transform_component>();
            transform.translation = glm::vec3(2.5f);
            this->m_camera.add_component<components::camera_component>().direction = glm::normalize(-transform.translation);
        }
    private:
        entity m_camera;
    };
    ref<application> get_application_instance() {
        return ref<model_loading_app>::create();
    }
}