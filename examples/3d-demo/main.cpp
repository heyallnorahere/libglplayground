#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE _3d_demo
#include <libglplayground.h>
#ifdef SYSTEM_MACOSX
constexpr int32_t major_opengl_version = 3;
#else
constexpr int32_t major_opengl_version = 4;
#endif
using namespace libplayground::gl;
namespace _3d_demo {
    struct vertex {
        glm::vec3 pos, color;
    };
    class _3d_demo_app : public application {
    public:
        _3d_demo_app() : application("3D demo", 800, 600, major_opengl_version) { }
    protected:
        virtual void load_content() override {
            std::vector<vertex_attribute> attributes = {
                { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, pos), false },
                { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, color), false }
            };
            std::vector<vertex> vertices = {
                // front
                { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.f, 0.f, 0.f) },
                { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.f, 0.f, 0.f) },
                { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.f, 0.f, 0.f) },
                { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.f, 0.f, 0.f) },

                // right side
                { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f) },
                { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.f, 1.f, 0.f) },
                { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.f, 1.f, 0.f) },
                { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f) },

                // left side
                { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.f, 0.f, 1.f) },
                { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f) },
                { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f) },
                { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.f, 0.f, 1.f) },

                // back
                { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.f, 1.f, 0.f) },
                { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.f, 1.f, 0.f) },
                { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.f, 1.f, 0.f) },
                { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.f, 1.f, 0.f) },

                // top
                { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.f, 1.f, 1.f) },
                { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.f, 1.f, 1.f) },
                { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.f, 1.f, 1.f) },
                { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.f, 1.f, 1.f) },

                // bottom
                { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.f, 0.f, 1.f) },
                { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.f, 0.f, 1.f) },
                { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.f, 0.f, 1.f) },
                { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.f, 0.f, 1.f) },
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
            this->m_vao = ref<vertex_array_object>::create();
            this->m_vbo = ref<vertex_buffer_object>::create(vertices);
            this->m_ebo = ref<element_buffer_object>::create(indices);
            this->m_vao->add_vertex_attributes(attributes);
            shader_factory factory;
            this->m_shader = factory.single_file("assets/shaders/3d-demo.glsl");
        }
        virtual void update() override {
            static float angle = 0.f;
            angle += 1.f;
            float aspect_ratio = (float)this->m_window->get_width() / (float)this->m_window->get_height();
            this->m_projection = glm::perspective(glm::radians(45.f), aspect_ratio, 0.1f, 100.f);
            this->m_view = glm::lookAt(glm::vec3(-2.5f, 2.5f, -2.5f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
            this->m_model = glm::rotate(glm::mat4(1.f), glm::radians(angle), glm::vec3(0.5f, 1.f, 0.5f));
        }
        virtual void render() override {
            this->m_shader->bind();
            this->m_shader->uniform_mat4("projection", this->m_projection);
            this->m_shader->uniform_mat4("view", this->m_view);
            this->m_shader->uniform_mat4("model", this->m_model);
            this->m_vao->bind();
            this->m_ebo->draw(GL_TRIANGLES);
            this->m_vao->unbind();
            this->m_shader->unbind();
        }
    private:
        ref<vertex_array_object> m_vao;
        ref<vertex_buffer_object> m_vbo;
        ref<element_buffer_object> m_ebo;
        ref<shader> m_shader;
        glm::mat4 m_projection, m_view, m_model;
    };
    ref<application> get_application_instance() {
        return ref<_3d_demo_app>::create();
    }
}