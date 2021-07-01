#pragma once
#include "ref.h"
// Huge credit goes to The Cherno (https://github.com/TheCherno) and his game engine for providing an example for skeletal animation.
namespace libplayground {
    namespace gl {
        struct vertex; // from renderer.h
        class shader;
        class texture;
        class vertex_array_object;
        class vertex_buffer_object;
        class element_buffer_object;
        struct vertex_bone_data {
            uint32_t ids[4] = { 0, 0, 0, 0 };
            float weights[4] = { 0.f, 0.f, 0.f, 0.f };
            void add_bone_data(uint32_t id, float weight) {
                for (size_t i = 0; i < 4; i++) {
                    if (this->weights[i] < 0.01f) { // floats arent always their assigned value, they vary between ~+0.00001f and ~-0.00001f
                        this->ids[i] = id;
                        this->weights[i] = weight;
                    }
                }
                spdlog::warn("This vertex has more than four bones and weights affecting it; extra data will not be used");
            }
        };
        class assimp_mesh {
        public:
            std::vector<vertex>& get_vertex_data();
            std::vector<uint32_t>& get_index_data();
            std::vector<vertex_bone_data>& get_bone_data();
            aiMesh* get_assimp_pointer();
            ref<vertex_array_object> get_vao();
            ref<vertex_buffer_object> get_vbo();
            ref<vertex_buffer_object> get_bone_buffer();
            ref<element_buffer_object> get_ebo();
            assimp_mesh(aiMesh* ptr, bool is_animated);
            assimp_mesh(const assimp_mesh&) = delete;
            assimp_mesh& operator=(const assimp_mesh&) = delete;
            void setup();
        private:
            std::vector<vertex> m_vertices;
            std::vector<uint32_t> m_indices;
            std::vector<vertex_bone_data> m_bone_data;
            aiMesh* m_ptr;
            bool m_is_animated;
            ref<vertex_array_object> m_vao;
            ref<vertex_buffer_object> m_vbo, m_bone_buffer;
            ref<element_buffer_object> m_ebo;
        };
        class model : public ref_counted {
        public:
            model(const std::string& path);
            model(const model&) = delete;
            model& operator=(const model&) = delete;
            std::vector<assimp_mesh>& get_meshes();
            const std::vector<assimp_mesh>& get_meshes() const;
            ref<shader> get_mesh_shader();
            const std::string& get_file_path() const;
            uint32_t get_animation_count() const;
            int32_t find_animation_by_name(const std::string& name) const;
            float get_animation_length(uint32_t index) const;
            void draw(int32_t animation_index = -1, float animation_time = 0.f);
            // todo: replace with a get_vertex_buffer, get_index_buffer, etc. functions when batch rendering comes along
        private:
            struct bone_info {
                glm::mat4 bone_offset, final_transform;
            };
            void bone_transform(float time, int32_t animation_index);
            void read_node_hierarchy(float animation_time, const aiNode* node, const glm::mat4& parent_transform, int32_t animation_index);
            void traverse_nodes(aiNode* node, const glm::mat4& parent_transform = glm::mat4(1.f), uint32_t level = 0);
            const aiNodeAnim* find_node_animation(const aiAnimation* animation, const std::string& node_name);
            uint32_t find_position(float animation_time, const aiNodeAnim* node_animation);
            uint32_t find_rotation(float animation_time, const aiNodeAnim* node_animation);
            uint32_t find_scale(float animation_time, const aiNodeAnim* node_animation);
            glm::vec3 interpolate_translation(float animation_time, const aiNodeAnim* node_animation);
            glm::quat interpolate_rotation(float animation_time, const aiNodeAnim* node_animation);
            glm::vec3 interpolate_scale(float animation_time, const aiNodeAnim* node_animation);
            std::unique_ptr<Assimp::Importer> m_importer;
            glm::mat4 m_inverse_transform;
            uint32_t m_bone_count = 0;
            std::vector<bone_info> m_bone_info;
            std::vector<assimp_mesh> m_meshes;
            std::unordered_map<std::string, uint32_t> m_bone_map;
            std::unordered_map<aiNode*, std::vector<uint32_t>> m_node_map;
            std::vector<glm::mat4> m_bone_transforms;
            const aiScene* m_scene;
            ref<shader> m_shader;
            std::string m_file_path;
            bool m_is_animated;
        };
    }
}