#define LIBGLPLAYGROUND_DEFINE_MAIN
#define LIBGLPLAYGROUND_APP_NAMESPACE basic_usage
#include <libglplayground.h>
#ifdef SYSTEM_MACOSX
constexpr int32_t major_opengl_version = 3;
#else
constexpr int32_t major_opengl_version = 4; // for debugging purposes on windows and linux
#endif
using namespace libplayground::gl;
namespace basic_usage {
    struct vertex {
        glm::vec3 pos, color;
    };
    class basic_usage_app : public application {
    public:
        basic_usage_app() : application("Example application", 800, 600, major_opengl_version) { }
    protected:
        virtual void load_content() override {
            std::vector<vertex_attribute> attributes = {
                { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, pos), false },
                { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, color), false }
            };
            std::vector<vertex> vertices = {
                { glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(1.f, 0.f, 0.f) },
                { glm::vec3( 0.5f, -0.5f, 0.f), glm::vec3(1.f, 1.f, 0.f) },
                { glm::vec3( 0.5f,  0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f) },
                { glm::vec3(-0.5f,  0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f) }
            };
            std::vector<uint32_t> indices = {
                0, 1, 3,
                1, 2, 3
            };
            this->m_vao = ref<vertex_array_object>::create();
            this->m_vbo = ref<vertex_buffer_object>::create(vertices);
            this->m_ebo = ref<element_buffer_object>::create(indices);
            this->m_vao->add_vertex_attributes(attributes);
            shader_factory factory;
            this->m_shader = factory.single_file("assets/shaders/basic-usage.glsl");
        }
        virtual void render() override {
            this->m_shader->bind();
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
    };
    ref<application> get_application_instance() {
        return ref<basic_usage_app>::create();
    }
}