#include "libglppch.h"
#include "renderer.h"
namespace libplayground {
    namespace gl {
        static std::vector<vertex_attribute> attributes = {
            { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, pos), false },
            { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, normal), false },
            { GL_FLOAT, 2, sizeof(vertex), offsetof(vertex, uv), false }
        };
        void renderer::reset() {
            this->m_shader.reset();
            this->m_meshes.clear();
        }
        void renderer::set_shader(ref<shader> shader) {
            this->m_shader = shader;
        }
        void renderer::submit(const mesh& m) {
            assembled_mesh assembled;
            assembled.transform = m.transform;
            assembled.vao = ref<vertex_array_object>::create();
            assembled.vbo = ref<vertex_buffer_object>::create(m.vertices);
            assembled.ebo = ref<element_buffer_object>::create(m.indices);
            assembled.vao->add_vertex_attributes(attributes);
            assembled.textures = m.textures;
            this->m_meshes.push_back(assembled);
        }
        void renderer::render() {
            // todo: instead of rendering each object individually, start batch rendering
            if (this->m_shader) {
                this->m_shader->bind();
            }
            for (auto& mesh : this->m_meshes) {
                if (this->m_shader) {
                    this->m_shader->uniform_mat4("model", mesh.transform);
                }
                for (size_t i = 0; i < mesh.textures.size(); i++) {
                    mesh.textures[i]->bind((uint32_t)i);
                }
                mesh.vao->bind();
                mesh.ebo->draw(GL_TRIANGLES);
                mesh.vao->unbind();
            }
        }
    }
}