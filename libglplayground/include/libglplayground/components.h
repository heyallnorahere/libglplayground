#pragma once
#include "renderer.h"
#include "model.h"
#include "script.h"
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
            struct model_component {
                ref<model> data;
                int32_t current_animation = -1;
            };

            struct script_component {
                std::vector<ref<script>> scripts;
                entity parent;
                template<typename T, typename... Args> void bind(Args&&... args) {
                    static_assert(std::is_base_of_v<script, T>, "The provided type does not extend \"script!\"");
                    ref<script> sc = ref<T>::create(std::forward(args)...);
                    sc->m_entity = this->parent;
                    this->scripts.push_back(sc);
                }
                void update() {
                    for (auto sc : this->scripts) {
                        sc->update();
                    }
                }
                script_component() = default;
                script_component(const script_component&) = default;
                script_component& operator=(const script_component&) = default;
            };
        }
        template<> inline void scene::on_component_added<components::script_component>(entity& ent, components::script_component& component) {
            component.parent = ent;
        }
    }
}