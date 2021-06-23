#include "libglppch.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array_object.h"
#include "vertex_buffer_object.h"
#include "element_buffer_object.h"
#include "model.h"
namespace libplayground {
    namespace gl {
        static glm::mat4 from_assimp_matrix(const aiMatrix4x4& matrix) {
            glm::mat4 result;
            result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
            result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
            result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
            result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
            return result;
        }
        template<size_t L, typename T> static glm::vec<L, float> from_assimp_vector(const T& vector) {
            glm::vec<L, float> result;
            for (size_t i = 0; i < L; i++) {
                result[i] = vector[i];
            }
            return result;
        }
        static constexpr uint32_t model_import_flags =
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_LimitBoneWeights;
        struct log_stream : public Assimp::LogStream {
            static void initialize() {
                if (Assimp::DefaultLogger::isNullLogger()) {
                    Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
                    Assimp::DefaultLogger::get()->attachStream(new log_stream, Assimp::Logger::Err | Assimp::Logger::Warn);
                }
            }
            virtual void write(const char* message) override {
                spdlog::error("Assimp: " + std::string(message));
            }
        };
        model::model(const std::string& path) {
            this->m_file_path = path;
            log_stream::initialize();
            spdlog::info("Loading model from: " + this->m_file_path);
            this->m_importer = std::make_unique<Assimp::Importer>();
            this->m_scene = this->m_importer->ReadFile(this->m_file_path, model_import_flags);
            if (!this->m_scene || !this->m_scene->HasMeshes()) {
                throw std::runtime_error("Could not load model from: " + this->m_file_path);
            }
            this->m_is_animated = this->m_scene->mAnimations != nullptr;
            // todo: get the model shader from the shader library, when thats implemented
            this->m_inverse_transform = glm::inverse(from_assimp_matrix(this->m_scene->mRootNode->mTransformation));
            uint32_t vertex_count = 0;
            uint32_t index_count = 0;
            this->m_meshes.reserve((size_t)this->m_scene->mNumMeshes);
            for (uint32_t m = 0; m < this->m_scene->mNumMeshes; m++) {
                aiMesh* mesh = this->m_scene->mMeshes[m];
                auto& mesh_ = this->m_meshes.emplace_back();
                mesh_.base_vertex = vertex_count;
                mesh_.base_index = index_count;
                mesh_.mesh_name = std::string(mesh->mName.C_Str());
                mesh_.vertex_count = mesh->mNumVertices;
                mesh_.index_count = mesh->mNumFaces * 3;
                mesh_.material_index = mesh->mMaterialIndex;
                vertex_count += mesh_.vertex_count;
                index_count += mesh_.index_count;
                if (!mesh->HasPositions()) {
                    throw std::runtime_error("This mesh does not have vertex positions!");
                }
                if (!mesh->HasNormals()) {
                    throw std::runtime_error("This mesh does not have normals!");
                }
                if (this->m_is_animated) {
                    for (size_t i = 0; i < (size_t)mesh->mNumVertices; i++) {
                        animated_vertex vertex;
                        vertex.m.pos = from_assimp_vector<3>(mesh->mVertices[i]);
                        vertex.m.normal = from_assimp_vector<3>(mesh->mNormals[i]);
                        if (mesh->HasTextureCoords(0)) {
                            vertex.m.uv = from_assimp_vector<2>(mesh->mTextureCoords[0][i]);
                        }
                        this->m_animated_vertices.push_back(vertex);
                    }
                } else {
                    for (size_t i = 0; i < (size_t)mesh->mNumVertices; i++) {
                        vertex v;
                        v.pos = from_assimp_vector<3>(mesh->mVertices[i]);
                        v.normal = from_assimp_vector<3>(mesh->mNormals[i]);
                        if (mesh->HasTextureCoords(0)) {
                            v.uv = from_assimp_vector<2>(mesh->mTextureCoords[0][i]);
                        }
                    }
                }
                for (size_t i = 0; i < (size_t)mesh->mNumFaces; i++) {
                    if (mesh->mFaces[i].mNumIndices != 3) {
                        throw std::runtime_error("Face does not have exactly 3 indices!");
                    }
                    index _i;
                    for (size_t j = 0; j < 3; j++) {
                        _i[j] = mesh->mFaces[i].mIndices[j];
                    }
                    this->m_indices.push_back(_i);
                    // todo: triangle cache
                }
            }
            this->traverse_nodes(this->m_scene->mRootNode);
            if (this->m_is_animated) {
                for (size_t m = 0; m < (size_t)this->m_scene->mNumMeshes; m++) {
                    aiMesh* mesh = this->m_scene->mMeshes[m];
                    auto& mesh_ = this->m_meshes[m];
                    for (size_t i = 0; i < (size_t)mesh->mNumBones; i++) {
                        aiBone* bone = mesh->mBones[i];
                        std::string bone_name = std::string(bone->mName.C_Str());
                        uint32_t bone_index = 0;
                        if (this->m_bone_map.find(bone_name) == this->m_bone_map.end()) {
                            bone_index = this->m_bone_count;
                            this->m_bone_count++;
                            this->m_bone_info.push_back(bone_info());
                            this->m_bone_info[bone_index].bone_offset = from_assimp_matrix(bone->mOffsetMatrix);
                            this->m_bone_map[bone_name] = bone_index;
                        } else {
                            bone_index = this->m_bone_map[bone_name];
                        }
                        for (size_t j = 0; j < (size_t)bone->mNumWeights; j++) {
                            uint32_t vertex_id = mesh_.base_index + bone->mWeights[j].mVertexId;
                            float weight = bone->mWeights[j].mWeight;
                            this->m_animated_vertices[vertex_id].add_bone_data(bone_index, weight);
                        }
                    }
                }
            }
            // todo: materials
            size_t stride = this->m_is_animated ? sizeof(animated_vertex) : sizeof(vertex);
            std::vector<vertex_attribute> attributes = {
                { GL_FLOAT, 3, stride, offsetof(vertex, pos), false },
                { GL_FLOAT, 3, stride, offsetof(vertex, normal), false },
                { GL_FLOAT, 2, stride, offsetof(vertex, uv), false }
            };
            this->m_vao = ref<vertex_array_object>::create();
            this->m_vao->bind();
            if (this->m_is_animated) {
                attributes.insert(attributes.end(), {
                    { GL_INT, 4, stride, offsetof(animated_vertex, ids), false },
                    { GL_FLOAT, 4, stride, offsetof(animated_vertex, weights), false }
                });
                this->m_vbo = ref<vertex_buffer_object>::create(this->m_animated_vertices);
            } else {
                this->m_vbo = ref<vertex_buffer_object>::create(this->m_static_vertices);
            }
            std::vector<uint32_t> indices;
            for (auto& index : this->m_indices) {
                for (size_t i = 0; i < 3; i++) {
                    indices.push_back(index[i]);
                }
            }
            this->m_ebo = ref<element_buffer_object>::create(indices);
            this->m_vao->unbind();
        }
        std::vector<animated_mesh>& model::get_meshes() {
            return this->m_meshes;
        }
        const std::vector<animated_mesh>& model::get_meshes() const {
            return this->m_meshes;
        }
        const std::vector<vertex>& model::get_static_vertices() const {
            return this->m_static_vertices;
        }
        const std::vector<index>& model::get_indices() const {
            return this->m_indices;
        }
        ref<shader> model::get_mesh_shader() {
            return this->m_shader;
        }
        std::vector<ref<texture>>& model::get_textures() {
            return this->m_textures;
        }
        const std::vector<ref<texture>>& model::get_textures() const {
            return this->m_textures;
        }
        const std::string& model::get_file_path() const {
            return this->m_file_path;
        }
        void model::bone_transform(float time) {
            // todo: implement
        }
        void model::read_node_hierarchy(float animation_time, const aiNode* node, const glm::mat4& parent_transform) {
            // todo: implement
        }
        void model::traverse_nodes(aiNode* node, const glm::mat4& parent_transform, uint32_t level) {
            // todo: implement
        }
        const aiNodeAnim* model::find_node_animation(const aiAnimation* animation, const std::string& node_name) {
            // todo: implement
            return nullptr;
        }
        uint32_t model::find_position(float animation_time, const aiNodeAnim* node_animation) {
            // todo: implement
            return 0;
        }
        uint32_t model::find_rotation(float animation_time, const aiNodeAnim* node_animation) {
            // todo: implement
            return 0;
        }
        uint32_t model::find_scale(float animation_time, const aiNodeAnim* node_animation) {
            // todo: implement
            return 0;
        }
        glm::vec3 model::interpolate_translation(float animation_time, const aiNodeAnim* node_animation) {
            // todo: implement
            return glm::vec3(0.f);
        }
        glm::quat model::interpolate_rotation(float animation_time, const aiNodeAnim* node_animation) {
            // todo: implement
            return glm::quat();
        }
        glm::vec3 model::interpolate_scale(float animation_time, const aiNodeAnim* node_animation) {
            // todo: implement
            return glm::vec3(0.f);
        }
    }
}