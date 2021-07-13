#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE user_input
#include <libglplayground.h>
#include <common/player_behavior.h>
#if !defined(SYSTEM_MACOSX) && !defined(NDEBUG)
constexpr int32_t major_opengl_version = 4;
#else
constexpr int32_t major_opengl_version = 3;
#endif
using namespace libplayground::gl;
namespace user_input {
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
            this->m_player.add_component<components::script_component>().bind<common::player_behavior>();
        }
        virtual void render() override {
#if defined(BUILT_IMGUI) && !defined(NDEBUG)
            {
                ImGui::Begin("Configuration");
                ImGui::SliderFloat("Mouse sensitivity", &common::player_behavior::sensitivity(), 0.f, 1.f);
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