#include "libglppch.h"
#include "window.h"
#include "renderer.h"
#include "entity.h"
#include "scene.h"
#include "application.h"
#include "components.h"
namespace libplayground {
    namespace gl {
        application::application(const std::string& title, int32_t width, int32_t height, int32_t major_opengl_version, int32_t minor_opengl_version) {
            this->m_title = title;
            this->m_window = ref<window>::create(title, width, height, major_opengl_version, minor_opengl_version);
            this->m_renderer = ref<renderer>::create();
            this->m_scene = ref<scene>::create();
        }
        void application::run() {
            spdlog::info("Starting application " + this->m_title + "...");
            this->load_content();
            while (!this->m_window->should_window_close()) {
                this->update();
                this->m_scene->update();
                this->m_window->clear();
                this->m_renderer->reset();
                this->render();
                this->m_scene->render(this->m_renderer, this->m_window);
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
    }
}