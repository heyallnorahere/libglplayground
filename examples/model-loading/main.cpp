#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE model_loading
#include <libglplayground.h>
#include <common/player_behavior.h>
#if !defined(SYSTEM_MACOSX) && !defined(NDEBUG)
constexpr int32_t major_opengl_version = 4;
#else
constexpr int32_t major_opengl_version = 3;
#endif
using namespace libplayground::gl;
namespace model_loading {
    class model_loading_app : public application {
    public:
        model_loading_app() : application("Model loading example", 800, 600, false, major_opengl_version) { }
    protected:
        virtual void load_content() override {
            shader_factory factory;
            auto& library = shader_library::get();
            library["model-animated"] = factory.multiple_files("assets/shaders/model-loading-animated.glsl", "assets/shaders/model-loading-fragment.glsl");
            library["model-static"] = factory.multiple_files("assets/shaders/model-loading-static.glsl", "assets/shaders/model-loading-fragment.glsl");
            this->m_entity = this->m_scene->create();
            this->m_entity.add_component<components::model_component>(ref<model>::create("assets/models/static-test-2.obj"), -1);
            this->m_camera = this->m_scene->create();
            this->m_camera.add_component<components::camera_component>().direction = glm::normalize(glm::vec3(-1.f));
            this->m_camera.add_component<components::script_component>().bind<common::player_behavior>();
            input_manager::get()->disable_mouse();
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
                auto& camera_transform = this->m_camera.get_component<components::transform_component>();
                auto& camera_component = this->m_camera.get_component<components::camera_component>();
                ImGui::InputFloat3("Camera position", &camera_transform.translation.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
                ImGui::InputFloat3("Camera direction", &camera_component.direction.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
                ImGui::End();
            }
#endif
        }
    private:
        entity m_entity, m_camera;
    };
    ref<application> get_application_instance() {
        return ref<model_loading_app>::create();
    }
}