#include "libglppch.h"
#include "window.h"
#include "application.h"
namespace libplayground {
    namespace gl {
        application::application(const std::string& title, int32_t width, int32_t height, int32_t major_opengl_version, int32_t minor_opengl_version) {
            this->m_title = title;
            this->m_window = ref<window>::create(title, width, height, major_opengl_version, minor_opengl_version);
        }
        void application::run() {
            spdlog::info("Starting application " + this->m_title + "...");
            this->load_content();
            while (!this->m_window->should_window_close()) {
                this->update();
                this->m_window->clear();
                this->render();
                this->m_window->swap_buffers();
                this->m_window->poll_events();
            }
            this->unload_content();
            spdlog::info("Shutting down...");
        }
        void application::load_content() { }
        void application::unload_content() { }
        void application::update() { }
        void application::render() { }
    }
}