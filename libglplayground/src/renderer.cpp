#include "libglppch.h"
#include "renderer.h"
#include "shader_library.h"
namespace libplayground {
    namespace gl {
        static std::vector<vertex_attribute> attributes = {
            { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, pos), false },
            { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, normal), false },
            { GL_FLOAT, 2, sizeof(vertex), offsetof(vertex, uv), false }
        };
        void renderer::reset() {
            this->m_models.clear();
            this->m_meshes.clear();
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
        void renderer::submit(const model_descriptor& model) {
            this->m_models.push_back(model);
        }
        void renderer::render() {
            // todo: instead of rendering each object individually, start batch rendering
            ref<shader> current_shader;
            auto& library = shader_library::get();
            std::string shader_name = "renderer-default";
            if (library.find(shader_name) != library.end()) {
                current_shader = library[shader_name];
            }
            if (current_shader) {
                current_shader->bind();
            }
            for (auto& mesh : this->m_meshes) {
                if (current_shader) {
                    current_shader->uniform_mat4("model", mesh.transform);
                }
                for (size_t i = 0; i < mesh.textures.size(); i++) {
                    auto& desc = mesh.textures[i];
                    desc.data->bind((uint32_t)i);
                    if (current_shader && !desc.uniform_name.empty()) {
                        current_shader->uniform_int(desc.uniform_name, (GLint)i);
                    }
                }
                mesh.vao->bind();
                mesh.ebo->draw(GL_TRIANGLES);
                mesh.vao->unbind();
            }
            for (const auto& model : this->m_models) {
                model.render_callback(model);
            }
        }
    }
}