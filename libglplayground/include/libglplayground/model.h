#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        struct vertex; // from renderer.h
        class shader;
        class texture;
        class vertex_array_object;
        class vertex_buffer_object;
        class element_buffer_object;
        struct animated_vertex {
            vertex m;
            uint32_t ids[4] = { 0, 0, 0, 0 };
            float weights[4] = { 0.f, 0.f, 0.f, 0.f };
            void add_bone_data(uint32_t id, float weight) {
                for (size_t i = 0; i < 4; i++) {
                    if (this->weights[i] < 0.01f) { // floats arent always their assigned value, they vary between ~+0.000000001f and ~-0.000000001f
                        this->ids[i] = id;
                        this->weights[i] = weight;
                    }
                }
                spdlog::warn("This vertex has more than four bones and weights affecting it; extra data will not be used");
            }
        };
        struct index {
            uint32_t v1, v2, v3;
            uint32_t& operator[](size_t index) {
                switch (index) {
                case 0:
                    return this->v1;
                    break;
                case 1:
                    return this->v2;
                    break;
                case 2:
                    return this->v3;
                    break;
                default:
                    throw std::runtime_error("Index out of range!");
                }
            }
        };
        struct animated_mesh {
            uint32_t base_vertex, base_index, material_index, index_count, vertex_count;
            glm::mat4 transform = glm::mat4(1.f);
            std::string node_name, mesh_name;
        };
        class model : public ref_counted {
        public:
            model(const std::string& path);
            model(const std::vector<vertex>& vertices, const std::vector<index>& indices, glm::mat4& transform);
            ~model();
            model(const model&) = delete;
            model& operator=(const model&) = delete;
            std::vector<animated_mesh>& get_meshes();
            const std::vector<animated_mesh>& get_meshes() const;
            const std::vector<vertex>& get_static_vertices() const;
            const std::vector<index>& get_indices() const;
            ref<shader> get_mesh_shader();
            std::vector<ref<texture>>& get_textures();
            const std::vector<ref<texture>>& get_textures() const;
            const std::string& get_file_path() const;
            // todo: add draw function, but replace with a get_vertex_buffer, get_index_buffer, etc. functions when batch rendering comes along
        private:
            struct bone_info {
                glm::mat4 bone_offset, final_transform;
            };
            void bone_transform(float time);
            void read_node_hierarchy(float animation_time, const aiNode* node, const glm::mat4& parent_transform);
            void traverse_nodes(aiNode* node, const glm::mat4& parent_transform = glm::mat4(1.f), uint32_t level = 0);
            const aiNodeAnim* find_node_animation(const aiAnimation* animation, const std::string& node_name);
            uint32_t find_position(float animation_time, const aiNodeAnim* node_animation);
            uint32_t find_rotation(float animation_time, const aiNodeAnim* node_animation);
            uint32_t find_scale(float animation_time, const aiNodeAnim* node_animation);
            glm::vec3 interpolate_translation(float animation_time, const aiNodeAnim* node_animation);
            glm::quat interpolate_rotation(float animation_time, const aiNodeAnim* node_animation);
            glm::vec3 interpolate_scale(float animation_time, const aiNodeAnim* node_animation);
            std::vector<animated_mesh> m_meshes;
            std::unique_ptr<Assimp::Importer> m_importer;
            glm::mat4 m_inverse_transform;
            uint32_t m_bone_count = 0;
            std::vector<bone_info> m_bone_info;
            ref<vertex_array_object> m_vao;
            ref<vertex_buffer_object> m_vbo;
            ref<element_buffer_object> m_ebo;
            std::vector<vertex> m_static_vertices;
            std::vector<animated_vertex> m_animated_vertices;
            std::vector<index> m_indices;
            std::unordered_map<std::string, uint32_t> m_bone_map;
            std::unordered_map<aiNode*, std::vector<uint32_t>> m_node_map;
            std::vector<glm::mat4> m_bone_transforms;
            const aiScene* m_scene;
            ref<shader> m_shader;
            std::vector<ref<texture>> m_textures;
            std::string m_file_path;
            bool m_is_animated;
        };
    }
}