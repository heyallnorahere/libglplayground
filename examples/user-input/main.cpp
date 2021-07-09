#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE user_input
#include <libglplayground.h>
#if !defined(SYSTEM_MACOSX) && !defined(NDEBUG)
constexpr int32_t major_opengl_version = 4;
#else
constexpr int32_t major_opengl_version = 3;
#endif
using namespace libplayground::gl;
namespace user_input {
    static float sensitivity = 0.1f;
    class player_behavior : public script {
    public:
        player_behavior() {
            this->m_last_mouse_offset = input_manager::get()->get_mouse();
        }
        virtual void update() override {
            this->update_camera_direction();
            auto& transform = this->m_entity.get_component<components::transform_component>();
            auto& camera = this->m_entity.get_component<components::camera_component>();
            ref<input_manager> im = input_manager::get();
            // todo: sync with framerate
            constexpr float camera_speed = 0.05f;
            if (im->get_key(key::W) & key_held) {
                transform.translation += camera_speed * camera.direction;
            }
            if (im->get_key(key::S) & key_held) {
                transform.translation -= camera_speed * camera.direction;
            }
            glm::vec3 unit_right = glm::normalize(glm::cross(camera.direction, camera.up));
            if (im->get_key(key::A) & key_held) {
                transform.translation -= camera_speed * unit_right;
            }
            if (im->get_key(key::D) & key_held) {
                transform.translation += camera_speed * unit_right;
            }
            if (im->get_key(key::O) & key_down) {
                im->enable_mouse();
            }
            if (im->get_key(key::P) & key_down) {
                im->disable_mouse();
            }
        }
    private:
        void update_camera_direction() {
            auto& camera = this->m_entity.get_component<components::camera_component>();
            glm::vec2 mouse_offset = input_manager::get()->get_mouse();
            glm::vec2 offset = (mouse_offset - this->m_last_mouse_offset) * glm::vec2(1.f, -1.f);
            this->m_last_mouse_offset = mouse_offset;
            glm::vec2 angle;
            angle.x = asin(camera.direction.y);
            angle.y = acos(camera.direction.x / cos(angle.x));
            angle += glm::radians(offset * sensitivity);
            glm::vec3 direction;
            direction.x = cos(angle.y) * cos(angle.x);
            direction.y = sin(angle.x);
            direction.z = sin(angle.y) * cos(angle.x);
            camera.direction = glm::normalize(direction);
        }
        glm::vec2 m_last_mouse_offset;
    };
    class user_input_app : public application {
    public:
        user_input_app() : application("User input example", 800, 600, false, major_opengl_version) { }
    protected:
        virtual void load_content() override {
            input_manager::get()->disable_mouse();
            shader_factory factory;
            auto& library = shader_library::get();
            library["renderer-default"] = factory.single_file("assets/shaders/user-input.glsl");
            entity player = this->m_scene->create();
            player.add_component<components::camera_component>();
            player.add_component<components::script_component>().bind<player_behavior>();
        }
        virtual void render() override {
#if defined(BUILT_IMGUI) && !defined(NDEBUG)
            {
                ImGui::Begin("Configuration");
                ImGui::SliderFloat("Mouse sensitivity", &sensitivity, 0.f, 1.f);
                ImGui::End();
            }
#endif
        }
    };
    ref<application> get_application_instance() {
        return ref<user_input_app>::create();
    }
}