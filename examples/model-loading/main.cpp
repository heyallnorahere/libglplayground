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
    static float sensitivity = 0.1f;
    class camera_behavior : public script {
    public:
        camera_behavior() {
            this->m_last_mouse = input_manager::get()->get_mouse();
        }
        virtual void update() override {
            this->calculate_camera_direction();
            auto& transform = this->m_entity.get_component<components::transform_component>();
            auto& camera = this->m_entity.get_component<components::camera_component>();
            ref<input_manager> im = input_manager::get();
            // todo: see user-input's player_behavior::update()
            constexpr float movement_speed = 0.05f;
            glm::vec3 unit_right = glm::normalize(glm::cross(camera.direction, camera.up));
            if (im->get_key(key::W) & key_held) {
                transform.translation += movement_speed * camera.direction;
            }
            if (im->get_key(key::S) & key_held) {
                transform.translation -= movement_speed * camera.direction;
            }
            if (im->get_key(key::A) & key_held) {
                transform.translation -= movement_speed * unit_right;
            }
            if (im->get_key(key::D) & key_held) {
                transform.translation += movement_speed * unit_right;
            }
            if (im->get_key(key::O) & key_down) {
                im->enable_mouse();
            }
            if (im->get_key(key::P) & key_down) {
                im->disable_mouse();
            }
            if (im->get_key(key::Q) & key_down) {
                application::get_running_application()->quit();
            }
        }
    private:
        void calculate_camera_direction() {
            auto& camera = this->m_entity.get_component<components::camera_component>();
            glm::vec2 mouse_position = input_manager::get()->get_mouse();
            glm::vec2 offset = (mouse_position - this->m_last_mouse) * glm::vec2(1.f, -1.f);
            this->m_last_mouse = mouse_position;
            offset *= sensitivity;
            glm::vec2 angle;
            angle.x = glm::degrees(asin(camera.direction.y));
            float factor = cos(glm::radians(angle.x));
            angle.y = glm::degrees(atan2(camera.direction.z / factor, camera.direction.x / factor));
            angle += glm::vec2(offset.y, offset.x);
            if (angle.x > 89.f) {
                angle.x = 89.f;
            }
            if (angle.x < -89.f) {
                angle.x = -89.f;
            }
            glm::vec3 direction;
            direction.x = cos(glm::radians(angle.y)) * cos(glm::radians(angle.x));
            direction.y = sin(glm::radians(angle.x));
            direction.z = sin(glm::radians(angle.y)) * cos(glm::radians(angle.x));
            camera.direction = glm::normalize(direction);
        }
        glm::vec2 m_last_mouse;
    };
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
            this->m_camera.add_component<components::script_component>().bind<camera_behavior>();
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