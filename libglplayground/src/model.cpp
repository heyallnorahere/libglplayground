#include "libglppch.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array_object.h"
#include "vertex_buffer_object.h"
#include "element_buffer_object.h"
#include "model.h"
#include "shader_library.h"
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
                result[(typename glm::vec<L, float>::length_type)i] = vector[(uint32_t)i];
            }
            return result;
        }
        static glm::quat from_assimp_quaternion(const aiQuaternion& quat) {
            return glm::quat(quat.w, quat.x, quat.y, quat.z);
        }
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
        std::vector<vertex>& assimp_mesh::get_vertex_data() {
            return this->m_vertices;
        }
        std::vector<uint32_t>& assimp_mesh::get_index_data() {
            return this->m_indices;
        }
        std::vector<vertex_bone_data>& assimp_mesh::get_bone_data() {
            return this->m_bone_data;
        }
        aiMesh* assimp_mesh::get_assimp_pointer() {
            return this->m_ptr;
        }
        ref<vertex_array_object> assimp_mesh::get_vao() {
            return this->m_vao;
        }
        ref<vertex_buffer_object> assimp_mesh::get_vbo() {
            return this->m_vbo;
        }
        ref<vertex_buffer_object> assimp_mesh::get_bone_buffer() {
            return this->m_bone_buffer;
        }
        ref<element_buffer_object> assimp_mesh::get_ebo() {
            return this->m_ebo;
        }
        std::string assimp_mesh::get_name() {
            return this->m_name;
        }
        void assimp_mesh::set_name(const std::string& name) {
            this->m_name = name;
        }
        assimp_mesh::assimp_mesh(aiMesh* ptr, bool is_animated) {
            this->m_ptr = ptr;
            this->m_is_animated = is_animated;
        }
        void assimp_mesh::setup() {
            this->m_vao = ref<vertex_array_object>::create();
            this->m_vao->bind();
            this->m_vbo = ref<vertex_buffer_object>::create(this->m_vertices);
            this->m_bone_buffer = ref<vertex_buffer_object>::create(this->m_bone_data);
            this->m_ebo = ref<element_buffer_object>::create(this->m_indices);
            std::vector<vertex_attribute> attributes = {
                { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, pos), false },
                { GL_FLOAT, 3, sizeof(vertex), offsetof(vertex, normal), false },
                { GL_FLOAT, 2, sizeof(vertex), offsetof(vertex, uv), false }
            };
            if (this->m_is_animated) {
                attributes.insert(attributes.end(), {
                    { GL_INT, 4, sizeof(vertex_bone_data), offsetof(vertex_bone_data, ids), false },
                    { GL_FLOAT, 4, sizeof(vertex_bone_data), offsetof(vertex_bone_data, weights), false },
                });
            }
            this->m_vao->add_vertex_attributes(attributes);
            this->m_vao->unbind();
        }
        model::model(const std::string& path) {
            this->m_file_path = path;
            log_stream::initialize();
            spdlog::info("Loading model from: " + this->m_file_path);
            this->m_importer = std::make_unique<Assimp::Importer>();
            uint32_t flags =
                aiProcess_Triangulate |
                aiProcess_FlipUVs |
                aiProcess_LimitBoneWeights;
            this->m_scene = this->m_importer->ReadFile(this->m_file_path, flags);
            if (!this->m_scene || !this->m_scene->HasMeshes() || this->m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
                throw std::runtime_error("Could not load model from: " + this->m_file_path);
            }
            this->m_is_animated = this->m_scene->mAnimations != nullptr;
            auto& library = shader_library::get();
            if (this->m_is_animated) {
                this->m_shader = library["model-animated"];
            } else {
                this->m_shader = library["model-static"];
            }
            this->m_inverse_transform = glm::inverse(from_assimp_matrix(this->m_scene->mRootNode->mTransformation));
            this->m_meshes.reserve((size_t)this->m_scene->mNumMeshes);
            for (uint32_t m = 0; m < this->m_scene->mNumMeshes; m++) {
                aiMesh* mesh = this->m_scene->mMeshes[m];
                auto& mesh_ = this->m_meshes.emplace_back(mesh, this->m_is_animated);
                if (!mesh->HasPositions()) {
                    throw std::runtime_error("This mesh does not have vertex positions!");
                }
                if (!mesh->HasNormals()) {
                    throw std::runtime_error("This mesh does not have normals!");
                }
                for (size_t i = 0; i < (size_t)mesh->mNumVertices; i++) {
                    vertex v;
                    v.pos = from_assimp_vector<3>(mesh->mVertices[i]);
                    v.normal = from_assimp_vector<3>(mesh->mNormals[i]);
                    if (mesh->HasTextureCoords(0)) {
                        v.uv = from_assimp_vector<2>(mesh->mTextureCoords[0][i]);
                    }
                    mesh_.get_vertex_data().push_back(v);
                }
                for (size_t i = 0; i < (size_t)mesh->mNumFaces; i++) {
                    if (mesh->mFaces[i].mNumIndices != 3) {
                        throw std::runtime_error("Face does not have exactly 3 indices!");
                    }
                    for (size_t j = 0; j < 3; j++) {
                        mesh_.get_index_data().push_back(mesh->mFaces[i].mIndices[j]);
                    }
                }
            }
            this->traverse_nodes(this->m_scene->mRootNode);
            if (this->m_is_animated) {
                for (size_t m = 0; m < (size_t)this->m_scene->mNumMeshes; m++) {
                    aiMesh* mesh = this->m_scene->mMeshes[m];
                    auto& mesh_ = this->m_meshes[m];
                    auto& bone_data = mesh_.get_bone_data();
                    bone_data.resize(mesh_.get_vertex_data().size());
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
                            uint32_t vertex_id = bone->mWeights[j].mVertexId;
                            float weight = bone->mWeights[j].mWeight;
                            bone_data[vertex_id].add_bone_data(bone_index, weight);
                        }
                    }
                }
            }
            // todo: materials
            for (auto& mesh : this->m_meshes) {
                mesh.setup(); // generate opengl buffers
            }
        }
        std::vector<assimp_mesh>& model::get_meshes() {
            return this->m_meshes;
        }
        const std::vector<assimp_mesh>& model::get_meshes() const {
            return this->m_meshes;
        }
        ref<shader> model::get_mesh_shader() {
            return this->m_shader;
        }
        const std::string& model::get_file_path() const {
            return this->m_file_path;
        }
        uint32_t model::get_animation_count() const {
            return this->m_scene->mNumAnimations;
        }
        int32_t model::find_animation_by_name(const std::string& name) const {
            if (this->m_is_animated) {
                for (uint32_t i = 0; i < this->m_scene->mNumAnimations; i++) {
                    const aiAnimation* animation = this->m_scene->mAnimations[i];
                    if (std::string(animation->mName.C_Str()) == name) {
                        return (int32_t)i;
                    }
                }
            }
            return -1;
        }
        float model::get_animation_length(uint32_t index) const {
            if (this->m_is_animated || index < this->m_scene->mNumAnimations) {
                const aiAnimation* animation = this->m_scene->mAnimations[index];
                return static_cast<float>(animation->mDuration / (animation->mTicksPerSecond != 0.0 ? animation->mTicksPerSecond : 25.0));
            }
            return 0.f;
        }
        void model::draw(int32_t animation_index, float animation_time) {
            if (!this->m_shader) {
                spdlog::warn("Model shader not found; make sure to set \"model-" + std::string(this->m_is_animated ? "animated" : "static") +  "\" in the shader library");
                return;
            }
            this->m_shader->bind();
            if (this->m_is_animated) {
                float time = 0.f;
                if (animation_index != -1) {
                    const aiAnimation* animation = this->m_scene->mAnimations[animation_index];
                    float ticks_per_second = static_cast<float>(animation->mTicksPerSecond != 0.0 ? animation->mTicksPerSecond : 25.0);
                    float time_in_ticks = animation_time * ticks_per_second;
                    time = fmod(time_in_ticks, (float)animation->mDuration);
                }
                // todo: calculate bones only once per entity per frame
                this->bone_transform(time, animation_index);
                for (size_t i = 0; i < this->m_bone_info.size(); i++) {
                    std::string uniform_name = "bones[" + std::to_string(i) + "]";
                    glm::mat4 matrix = this->m_bone_transforms[i];
                    this->m_shader->uniform_mat4(uniform_name, matrix);
                }
            }
            for (auto& mesh : this->m_meshes) {
                auto vao = mesh.get_vao();
                vao->bind();
                mesh.get_ebo()->draw(GL_TRIANGLES);
                vao->unbind();
            }
        }
        void model::bone_transform(float time, int32_t animation_index) {
            this->read_node_hierarchy(time, this->m_scene->mRootNode, glm::mat4(1.f), animation_index);
            this->m_bone_transforms.resize(this->m_bone_count);
            for (size_t i = 0; i < (size_t)this->m_bone_count; i++) {
                this->m_bone_transforms[i] = this->m_bone_info[i].final_transform;
            }
        }
        void model::read_node_hierarchy(float animation_time, const aiNode* node, const glm::mat4& parent_transform, int32_t animation_index) {
            std::string name = std::string(node->mName.C_Str());
            const aiAnimation* animation = nullptr;
            if (animation_index != -1) {
                animation = this->m_scene->mAnimations[animation_index];
            }
            glm::mat4 node_transform = from_assimp_matrix(node->mTransformation);
            const aiNodeAnim* node_animation = nullptr;
            if (animation) {
                node_animation = this->find_node_animation(animation, name);
            }
            if (node_animation) {
                glm::vec3 translation = this->interpolate_translation(animation_time, node_animation);
                glm::mat4 translation_matrix = glm::translate(glm::mat4(1.f), translation);
                glm::quat rotation = this->interpolate_rotation(animation_time, node_animation);
                glm::mat4 rotation_matrix = glm::toMat4(rotation);
                glm::vec3 scale = this->interpolate_scale(animation_time, node_animation);
                glm::mat4 scale_matrix = glm::scale(glm::mat4(1.f), scale);
                node_transform = translation_matrix * rotation_matrix * scale_matrix;
            }
            glm::mat4 transform = parent_transform * node_transform;
            if (this->m_bone_map.find(name) != this->m_bone_map.end()) {
                uint32_t bone_index = this->m_bone_map[name];
                bone_info& bi = this->m_bone_info[(size_t)bone_index];
                bi.final_transform = this->m_inverse_transform * transform * bi.bone_offset;
            }
            for (uint32_t i = 0; i < node->mNumChildren; i++) {
                this->read_node_hierarchy(animation_time, node->mChildren[i], transform, animation_index);
            }
        }
        void model::traverse_nodes(aiNode* node, const glm::mat4& parent_transform, uint32_t level) {
            glm::mat4 transform = parent_transform * from_assimp_matrix(node->mTransformation);
            this->m_node_map[node].resize(node->mNumMeshes);
            for (uint32_t i = 0; i < node->mNumMeshes; i++) {
                uint32_t mesh = node->mMeshes[i];
                assimp_mesh& am = this->m_meshes[mesh];
                am.set_name(node->mName.C_Str());
                this->m_node_map[node][i] = mesh;
            }
            for (uint32_t i = 0; i < node->mNumChildren; i++) {
                this->traverse_nodes(node->mChildren[i], transform, level + 1);
            }
        }
        const aiNodeAnim* model::find_node_animation(const aiAnimation* animation, const std::string& node_name) {
            for (uint32_t i = 0; i < animation->mNumChannels; i++) {
                const aiNodeAnim* node_animation = animation->mChannels[i];
                if (std::string(node_animation->mNodeName.C_Str()) == node_name) {
                    return node_animation;
                }
            }
            return nullptr;
        }
        uint32_t model::find_position(float animation_time, const aiNodeAnim* node_animation) {
            for (uint32_t i = 0; i < node_animation->mNumPositionKeys - 1; i++) {
                if (animation_time < (float)node_animation->mPositionKeys[i + 1].mTime) {
                    return i;
                }
            }
            return 0;
        }
        uint32_t model::find_rotation(float animation_time, const aiNodeAnim* node_animation) {
            for (uint32_t i = 0; i < node_animation->mNumRotationKeys - 1; i++) {
                if (animation_time < (float)node_animation->mRotationKeys[i + 1].mTime) {
                    return i;
                }
            }
            return 0;
        }
        uint32_t model::find_scale(float animation_time, const aiNodeAnim* node_animation) {
            for (uint32_t i = 0; i < node_animation->mNumScalingKeys - 1; i++) {
                if (animation_time < (float)node_animation->mScalingKeys[i + 1].mTime) {
                    return i;
                }
            }
            return 0;
        }
        glm::vec3 model::interpolate_translation(float animation_time, const aiNodeAnim* node_animation) {
            if (node_animation->mNumPositionKeys <= 1) {
                return from_assimp_vector<3>(node_animation->mPositionKeys[0].mValue);
            }
            uint32_t position_index = this->find_position(animation_time, node_animation);
            uint32_t next_position_index = position_index + 1;
            float delta_time = (float)(node_animation->mPositionKeys[next_position_index].mTime - node_animation->mPositionKeys[position_index].mTime);
            float factor = (delta_time - (float)node_animation->mPositionKeys[position_index].mTime) / delta_time;
            if (factor > 1.f) {
                throw std::runtime_error("Factor must be below 1!");
            }
            factor = glm::clamp(factor, 0.f, 1.f);
            const auto& start = node_animation->mPositionKeys[position_index].mValue;
            const auto& end = node_animation->mPositionKeys[next_position_index].mValue;
            auto delta = end - start;
            return from_assimp_vector<3>(start + factor * delta);
        }
        glm::quat model::interpolate_rotation(float animation_time, const aiNodeAnim* node_animation) {
            if (node_animation->mNumRotationKeys <= 1) {
                return from_assimp_quaternion(node_animation->mRotationKeys[0].mValue);
            }
            uint32_t rotation_index = this->find_rotation(animation_time, node_animation);
            uint32_t next_rotation_index = rotation_index + 1;
            float delta_time = (float)(node_animation->mRotationKeys[next_rotation_index].mTime - node_animation->mRotationKeys[rotation_index].mTime);
            float factor = (delta_time - (float)node_animation->mRotationKeys[rotation_index].mTime) / delta_time;
            if (factor > 1.f) {
                throw std::runtime_error("Factor must be below 1!");
            }
            factor = glm::clamp(factor, 0.f, 1.f);
            const auto& start = node_animation->mRotationKeys[rotation_index].mValue;
            const auto& end = node_animation->mRotationKeys[next_rotation_index].mValue;
            aiQuaternion q;
            aiQuaternion::Interpolate(q, start, end, factor);
            return from_assimp_quaternion(q.Normalize());
        }
        glm::vec3 model::interpolate_scale(float animation_time, const aiNodeAnim* node_animation) {
            if (node_animation->mNumScalingKeys <= 1) {
                return from_assimp_vector<3>(node_animation->mScalingKeys[0].mValue);
            }
            uint32_t scale_index = this->find_scale(animation_time, node_animation);
            uint32_t next_scale_index = scale_index + 1;
            float delta_time = (float)(node_animation->mScalingKeys[next_scale_index].mTime - node_animation->mScalingKeys[scale_index].mTime);
            float factor = (delta_time - (float)node_animation->mScalingKeys[scale_index].mTime) / delta_time;
            if (factor > 1.f) {
                throw std::runtime_error("Factor must be below 1!");
            }
            factor = glm::clamp(factor, 0.f, 1.f);
            const auto& start = node_animation->mScalingKeys[scale_index].mValue;
            const auto& end = node_animation->mScalingKeys[next_scale_index].mValue;
            auto delta = end - start;
            return from_assimp_vector<3>(start + factor * delta);
        }
    }
}