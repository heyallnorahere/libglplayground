#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE ecs_example
#include <libglplayground.h>
#if !defined(SYSTEM_MACOSX) && !defined(NDEBUG)
constexpr int32_t major_opengl_version = 4;
#else
constexpr int32_t major_opengl_version = 3;
#endif
using namespace libplayground::gl;
namespace ecs_example {
    class camera_behavior : public script {
    public:
        virtual void update() override {
            static float angle = 0.f;
            angle += 1.f;
            float factor = 5.f;
            float x = cos(glm::radians(angle)) * factor;
            float z = sin(glm::radians(angle)) * factor;
            auto& transform = this->m_entity.get_component<components::transform_component>();
            transform.translation = glm::vec3(x, 0.f, z);
            auto& camera = this->m_entity.get_component<components::camera_component>();
            camera.direction = glm::normalize(-transform.translation);
        }
    };
    class ecs_example_app : public application {
    public:
        ecs_example_app() : application("ECS example", 800, 600, false, major_opengl_version) { }
    protected:
        virtual void load_content() override {
            // vertex positions are from 3d-demo
            std::vector<vertex> vertices = {
                // front
                { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.f,  0.f, -1.f), glm::vec2(0.f, 0.f) },
                { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.f,  0.f, -1.f), glm::vec2(1.f, 0.f) },
                { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.f,  0.f, -1.f), glm::vec2(1.f, 1.f) },
                { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.f,  0.f, -1.f), glm::vec2(0.f, 1.f) },

                // right side
                { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 1.f,  0.f,  0.f), glm::vec2(0.f, 0.f) },
                { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 1.f,  0.f,  0.f), glm::vec2(1.f, 0.f) },
                { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 1.f,  0.f,  0.f), glm::vec2(1.f, 1.f) },
                { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 1.f,  0.f,  0.f), glm::vec2(0.f, 1.f) },

                // left side
                { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.f,  0.f,  0.f), glm::vec2(0.f, 0.f) },
                { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.f,  0.f,  0.f), glm::vec2(1.f, 0.f) },
                { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.f,  0.f,  0.f), glm::vec2(1.f, 1.f) },
                { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.f,  0.f,  0.f), glm::vec2(0.f, 1.f) },

                // back
                { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.f,  0.f,  1.f), glm::vec2(0.f, 0.f) },
                { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.f,  0.f,  1.f), glm::vec2(1.f, 0.f) },
                { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.f,  0.f,  1.f), glm::vec2(1.f, 1.f) },
                { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.f,  0.f,  1.f), glm::vec2(0.f, 1.f) },

                // top
                { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.f,  1.f,  0.f), glm::vec2(0.f, 0.f) },
                { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.f,  1.f,  0.f), glm::vec2(1.f, 0.f) },
                { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.f,  1.f,  0.f), glm::vec2(1.f, 1.f) },
                { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.f,  1.f,  0.f), glm::vec2(0.f, 1.f) },

                // bottom
                { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.f, -1.f,  0.f), glm::vec2(0.f, 0.f) },
                { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.f, -1.f,  0.f), glm::vec2(1.f, 0.f) },
                { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.f, -1.f,  0.f), glm::vec2(1.f, 1.f) },
                { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.f, -1.f,  0.f), glm::vec2(0.f, 1.f) },
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
            std::vector<glm::vec3> positions = {
                glm::vec3(-1.f),
                glm::vec3(0.f),
                glm::vec3(1.f)
            };
            std::vector<ref<texture>> textures = {
                texture::from_file("assets/textures/tex1.png"),
                texture::from_file("assets/textures/tex2.png")
            };
            for (size_t i = 0; i < positions.size(); i++) {
                glm::vec3 pos = positions[i];
                auto entity = this->m_scene->create();
                entity.get_component<components::transform_component>().translation = pos;
                std::vector<texture_descriptor> tex = { { { textures[i % textures.size()] }, "tex" } };
                entity.add_component<components::mesh_component>(vertices, indices, tex);
            }
            this->m_camera = this->m_scene->create();
            this->m_camera.add_component<components::camera_component>();
            this->m_camera.add_component<components::script_component>().bind<camera_behavior>();
            shader_factory factory;
            auto& library = shader_library::get();
            library["renderer-default"] = factory.single_file("assets/shaders/ecs-example.glsl");
        }
    private:
        ref<shader> m_shader;
        entity m_camera;
    };
    ref<application> get_application_instance() {
        return ref<ecs_example_app>::create();
    }
}