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
            library["model-animated"] = factory.multiple_files("assets/shaders/model-loading-static.glsl", "assets/shaders/model-loading-fragment.glsl");
            this->m_entity = this->m_scene->create();
            this->m_entity.add_component<components::model_component>(ref<model>::create("assets/models/bee.glb"), -1);
            entity camera = this->m_scene->create();
            auto& transform = camera.get_component<components::transform_component>();
            transform.translation = glm::vec3(5.f);
            camera.add_component<components::camera_component>().direction = glm::normalize(-transform.translation);
        }
        virtual void render() override {
#if defined(BUILT_IMGUI) && !defined(NDEBUG)
            {
                ImGui::Begin("Debug menu");
                auto& transform = this->m_entity.get_component<components::transform_component>();
                auto& model = this->m_entity.get_component<components::model_component>();
                ImGui::DragFloat3("Position", &transform.translation.x);
                ImGui::DragFloat3("Rotation", &transform.rotation.x);
                ImGui::DragFloat3("Scale", &transform.scale.x, 0.05f, 0.001f, 2.f);
                ImGui::InputInt("Animation ID", &model.current_animation);
                if (model.current_animation < -1) {
                    model.current_animation = -1;
                }
                int32_t animation_count = (int32_t)model.data->get_animation_count();
                if (model.current_animation >= animation_count) {
                    model.current_animation = animation_count - 1;
                }
                ImGui::End();
            }
#endif
        }
    private:
        entity m_entity;
    };
    ref<application> get_application_instance() {
        return ref<model_loading_app>::create();
    }
}