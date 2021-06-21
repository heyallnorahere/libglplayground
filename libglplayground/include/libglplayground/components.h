#pragma once
#include "renderer.h"
namespace libplayground {
    namespace gl {
        namespace components {
            struct transform_component {
                glm::vec3 translation = glm::vec3(0.f);
                glm::vec3 rotation = glm::vec3(0.f);
                glm::vec3 scale = glm::vec3(1.f);
                transform_component() = default;
                transform_component(const transform_component&) = default;
                transform_component(const glm::vec3& translation) {
                    this->translation = translation;
                }
                transform_component& operator=(const transform_component&) = default;
                glm::mat4 get_matrix() {
                    glm::mat4 rotation = glm::toMat4(glm::quat(this->rotation));
                    return glm::translate(glm::mat4(1.f), this->translation) * rotation * glm::scale(glm::mat4(1.f), this->scale);
                }
            };
            struct mesh_component {
                std::vector<vertex> vertices;
                std::vector<uint32_t> indices;
                std::vector<texture_descriptor> textures;
                mesh_component() = default;
                mesh_component(const mesh_component&) = default;
                mesh_component& operator=(const mesh_component&) = default;
            };
            struct camera_component {
                glm::vec3 direction, up;
                bool primary;
                camera_component() {
                    this->direction = glm::vec3(0.f, 0.f, 1.f);
                    this->up = glm::vec3(0.f, 1.f, 0.f);
                    this->primary = true;
                }
                camera_component(const camera_component&) = default;
                camera_component& operator=(const camera_component&) = default;
            };
        }
    }
}