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
        void application::update_components() {
            // todo: update components (i.e. scripts)
        }
        void application::render_components() {
            auto view = this->m_registry.view<components::transform_component, components::mesh_component>();
            view.each([&](auto& transform, auto& mesh) {
                ::libplayground::gl::mesh m;
                m.transform = transform.transform;
                m.vertices = mesh.vertices;
                m.indices = mesh.indices;
                this->m_renderer->submit(m);
            });
        }
    }
}