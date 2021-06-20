#include "libglppch.h"
#include "window.h"
#include "renderer.h"
#include "application.h"
#include "components.h"
namespace libplayground {
    namespace gl {
        application::application(const std::string& title, int32_t width, int32_t height, int32_t major_opengl_version, int32_t minor_opengl_version) {
            this->m_title = title;
            this->m_window = ref<window>::create(title, width, height, major_opengl_version, minor_opengl_version);
            this->m_renderer = ref<renderer>::create();
        }
        void application::run() {
            spdlog::info("Starting application " + this->m_title + "...");
            this->load_content();
            while (!this->m_window->should_window_close()) {
                this->update();
                this->update_components();
                this->m_window->clear();
                this->m_renderer->reset();
                this->render();
                this->render_components();
                this->m_renderer->render();
                this->m_window->swap_buffers();
                this->m_window->poll_events();
            }
            spdlog::info("Shutting down...");
            this->unload_content();
        }
        void application::load_content() { }
        void application::unload_content() { }
        void application::update() { }
        void application::render() { }
        void application::update_components() { }
        void application::render_components() {
            auto renderable_view = this->m_registry.view<components::transform_component, components::mesh_component>();
            renderable_view.each([&](auto& transform, auto& mesh) {
                ::libplayground::gl::mesh m;
                m.transform = transform.get_matrix();
                m.vertices = mesh.vertices;
                m.indices = mesh.indices;
                m.textures = mesh.textures;
                this->m_renderer->submit(m);
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
                ref<shader> current_shader = this->m_renderer->get_shader();
                if (current_shader) {
                    float aspect_ratio = (float)this->m_window->get_width() / (float)this->m_window->get_height();
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