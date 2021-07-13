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
            if (im->get_key(key::Q) & key_down) {
                application::get_running_application()->quit();
            }
        }
    private:
        void update_camera_direction() {
            auto& camera = this->m_entity.get_component<components::camera_component>();
            glm::vec2 mouse_offset = input_manager::get()->get_mouse();
            glm::vec2 offset = (mouse_offset - this->m_last_mouse_offset) * glm::vec2(1.f, -1.f);
            offset *= sensitivity;
            this->m_last_mouse_offset = mouse_offset;
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
        glm::vec2 m_last_mouse_offset;
    };
    class user_input_app : public application {
    public:
        user_input_app() : application("User input example", 800, 600, false, major_opengl_version) { }
    protected:
        virtual void load_content() override {
            // vertex positions are from 3d-demo
            std::vector<vertex> vertices = {
                // front
                { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f,  0.f, -1.f), glm::vec2(0.f, 0.f) },
                { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f,  0.f, -1.f), glm::vec2(1.f, 0.f) },
                { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.f,  0.f, -1.f), glm::vec2(1.f, 1.f) },
                { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.f,  0.f, -1.f), glm::vec2(0.f, 1.f) },

                // right side
                { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.f,  0.f,  0.f), glm::vec2(0.f, 0.f) },
                { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.f,  0.f,  0.f), glm::vec2(1.f, 0.f) },
                { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.f,  0.f,  0.f), glm::vec2(1.f, 1.f) },
                { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.f,  0.f,  0.f), glm::vec2(0.f, 1.f) },

                // left side
                { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.f,  0.f,  0.f), glm::vec2(0.f, 0.f) },
                { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.f,  0.f,  0.f), glm::vec2(1.f, 0.f) },
                { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.f,  0.f,  0.f), glm::vec2(1.f, 1.f) },
                { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.f,  0.f,  0.f), glm::vec2(0.f, 1.f) },

                // back
                { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.f,  0.f,  1.f), glm::vec2(0.f, 0.f) },
                { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.f,  0.f,  1.f), glm::vec2(1.f, 0.f) },
                { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.f,  0.f,  1.f), glm::vec2(1.f, 1.f) },
                { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.f,  0.f,  1.f), glm::vec2(0.f, 1.f) },

                // top
                { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.f,  1.f,  0.f), glm::vec2(0.f, 0.f) },
                { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.f,  1.f,  0.f), glm::vec2(1.f, 0.f) },
                { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.f,  1.f,  0.f), glm::vec2(1.f, 1.f) },
                { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.f,  1.f,  0.f), glm::vec2(0.f, 1.f) },

                // bottom
                { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, -1.f,  0.f), glm::vec2(0.f, 0.f) },
                { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, -1.f,  0.f), glm::vec2(1.f, 0.f) },
                { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.f, -1.f,  0.f), glm::vec2(1.f, 1.f) },
                { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.f, -1.f,  0.f), glm::vec2(0.f, 1.f) },
            };
            std::vector<uint32_t> indices;
            for (uint32_t i = 0; i < 6; i++) {
                uint32_t offset = i * 4;
                std::vector<uint32_t> quad = {
                    0, 1, 3,
                    1, 2, 3
                };
                for (uint32_t& index : quad) {
                    index += offset;
                }
                indices.insert(indices.end(), quad.begin(), quad.end());
            }
            this->m_object = this->m_scene->create();
            this->m_object.add_component<components::mesh_component>(vertices, indices, std::vector<texture_descriptor>());
            this->m_object.get_component<components::transform_component>().translation = glm::vec3(5.f, 0.f, 0.f);
            input_manager::get()->disable_mouse();
            shader_factory factory;
            auto& library = shader_library::get();
            library["renderer-default"] = factory.single_file("assets/shaders/user-input.glsl");
            this->m_player = this->m_scene->create();
            this->m_player.add_component<components::camera_component>();
            this->m_player.add_component<components::script_component>().bind<player_behavior>();
        }
        virtual void render() override {
#if defined(BUILT_IMGUI) && !defined(NDEBUG)
            {
                ImGui::Begin("Configuration");
                ImGui::SliderFloat("Mouse sensitivity", &sensitivity, 0.f, 1.f);
                auto& camera = this->m_player.get_component<components::camera_component>();
                auto& player_transform = this->m_player.get_component<components::transform_component>();
                auto& object_transform = this->m_object.get_component<components::transform_component>();
                ImGui::InputFloat3("Camera direction", &camera.direction.x);
                ImGui::InputFloat3("Player position", &player_transform.translation.x);
                ImGui::InputFloat3("Object position", &object_transform.translation.x);
                ImGui::End();
            }
#endif
        }
    private:
        entity m_player, m_object;
    };
    ref<application> get_application_instance() {
        return ref<user_input_app>::create();
    }
}