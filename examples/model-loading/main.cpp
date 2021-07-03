#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE model_loading
#include <libglplayground.h>
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
            this->m_entity = this->m_scene->create();
            this->m_entity.add_component<components::model_component>(ref<model>::create("assets/models/bee.glb"), -1);
            this->m_camera = this->m_scene->create();
            this->m_camera.add_component<components::camera_component>().direction = glm::normalize(glm::vec3(-1.f));
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
                static float distance_from_object = 5.f;
                static float last_distance = 0.f;
                ImGui::SliderFloat("Distance from object", &distance_from_object, 1.f, 100.f);
                if (fabs(distance_from_object - last_distance) > 0.001f) {
                    this->m_camera.get_component<components::transform_component>().translation = glm::vec3(distance_from_object);
                    last_distance = distance_from_object;
                }
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