#pragma once
#include "ref.h"
#include "shader.h"
#include "vertex_array_object.h"
#include "vertex_buffer_object.h"
#include "element_buffer_object.h"
#include "texture.h"
namespace libplayground {
    namespace gl {
        struct vertex {
            glm::vec3 pos, normal;
            glm::vec2 uv;
        };
        struct texture_descriptor {
            ref<texture> data;
            std::string uniform_name;
        };
        struct mesh {
            glm::mat4 transform;
            std::vector<vertex> vertices;
            std::vector<uint32_t> indices;
            std::vector<texture_descriptor> textures;
        };
        struct model_descriptor {
            std::function<void(const model_descriptor&)> render_callback; // todo: not this
            glm::mat4 transform;
            int32_t animation_id = -1;
        };
        class renderer : public ref_counted {
        public:
            void reset();
            void submit(const mesh& m);
            void submit(const model_descriptor& model);
            void render();
        private:
            struct assembled_mesh {
                std::vector<texture_descriptor> textures;
                ref<vertex_array_object> vao;
                ref<vertex_buffer_object> vbo;
                ref<element_buffer_object> ebo;
                glm::mat4 transform;
            };
            std::vector<assembled_mesh> m_meshes;
            std::vector<model_descriptor> m_models;
        };
    }
}