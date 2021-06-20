#include "libglppch.h"
#include "renderer.h"
#include "window.h"
#include "entity.h"
#include "scene.h"
#include "components.h"
namespace libplayground {
    namespace gl {
        entity scene::create() {
            entity entity(this->m_registry.create(), this);
            entity.add_component<components::transform_component>();
            return entity;
        }
        void scene::destroy(const entity& entity) {
            this->m_registry.destroy(entity);
        }
        void scene::update() {
            // todo: update components
        }
        void scene::render(ref<renderer> renderer, ref<window> window) {
            auto renderable_view = this->m_registry.view<components::transform_component, components::mesh_component>();
            renderable_view.each([&](auto& transform, auto& mesh) {
                ::libplayground::gl::mesh m;
                m.transform = transform.get_matrix();
                m.vertices = mesh.vertices;
                m.indices = mesh.indices;
                m.textures = mesh.textures;
                renderer->submit(m);
            });
            auto camera_view = this->m_registry.view<components::transform_component, components::camera_component>();
            entt::entity camera = entt::null;
            // first, search for primary camera entities
            camera_view.each([&](const auto& entity, auto& transform, auto& camera_) {
                if (camera_.primary) {
                    camera = entity;
                }
            });
            // next, if no primary camera was found, get the first camera in the registry
            if (camera == entt::null) {
                camera = camera_view.front();
            }
            // and then, if we found a camera, calculate matricies for rendering
            if (camera != entt::null) {
                ref<shader> current_shader = renderer->get_shader();
                if (current_shader) {
                    float aspect_ratio = (float)window->get_width() / (float)window->get_height();
                    auto components = camera_view.get(camera);
                    auto& transform = std::get<0>(components);
                    glm::vec3 position = transform.get_matrix() * glm::vec4(0.f, 0.f, 0.f, 1.f);
                    auto& camera_comp = std::get<1>(components);
                    glm::mat4 projection = glm::perspective(glm::radians(45.f), aspect_ratio, 0.1f, 100.f); // todo: make every field part of camera_component
                    glm::mat4 view = glm::lookAt(position, position + camera_comp.direction, camera_comp.up);
                    current_shader->bind();
                    current_shader->uniform_mat4("projection", projection);
                    current_shader->uniform_mat4("view", view);
                }
            }
        }
    }
}