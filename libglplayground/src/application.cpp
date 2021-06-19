#include "libglppch.h"
#include "application.h"
namespace libplayground {
    namespace gl {
        application::application(const std::string& title, int32_t width, int32_t height, int32_t major_opengl_version, int32_t minor_opengl_version) {
            this->m_title = title;
        }
        void application::run() {
            spdlog::info("Starting application " + this->m_title + "...");
            this->load_content();
            // todo: game loop
            this->unload_content();
            spdlog::info("Shutting down...");
        }
    }
}