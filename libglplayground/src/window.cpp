#include "libglppch.h"
#include "window.h"
static uint32_t window_count = 0;
namespace libplayground {
    namespace gl {
        static void opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* user_param) {
            switch (id) {
            case 131169:
            case 131185:
            case 131218:
            case 131204:
                return;    
            }
            std::string formatted_message = "OpenGL: " + std::string(message);
            switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                throw std::runtime_error(formatted_message);
                break;
            default:
                spdlog::info(formatted_message);
                break;
            }
        }
        window::window(const std::string& title, int32_t width, int32_t height, int32_t major_opengl_version, int32_t minor_opengl_version) {
            if (window_count == 0) {
                if (!glfwInit()) {
                    throw std::runtime_error("GLFW failed to initialize!");
                }
            }
            window_count++;
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_opengl_version);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_opengl_version);
#ifndef NDEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
            this->m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
            if (!this->m_window) {
                const char* desc;
                glfwGetError(&desc);
                throw std::runtime_error("Failed to create GLFW window: " + std::string(desc));
            }
            this->m_title = title;
            this->m_width = width;
            this->m_height = height;
            glfwMakeContextCurrent(this->m_window);
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            GLint flags;
            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            bool debug = flags & GL_CONTEXT_FLAG_DEBUG_BIT;
            if (debug && major_opengl_version >= 4 && minor_opengl_version >= 3) {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(opengl_debug_callback, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
            }
            spdlog::info("Successfully created window and OpenGL context!");
            spdlog::info("\tWidth: " + std::to_string(this->m_width));
            spdlog::info("\tHeight: " + std::to_string(this->m_height));
            spdlog::info("\tContext version: " + std::string((char*)glGetString(GL_VERSION)) + (debug ? " (debug)" : ""));
        }
        window::~window() {
            glfwDestroyWindow(this->m_window);
            window_count--;
            if (window_count == 0) {
                glfwTerminate();
            }
        }
        GLFWwindow* window::get() {
            return this->m_window;
        }
        std::string window::get_title() {
            return this->m_title;
        }
        int32_t window::get_width() {
            return this->m_width;
        }
        int32_t window::get_height() {
            return this->m_height;
        }
        bool window::should_window_close() {
            return glfwWindowShouldClose(this->m_window);
        }
        void window::clear() {
            glClear(GL_COLOR_BUFFER_BIT); // todo: add more flags later
        }
        void window::swap_buffers() {
            glfwSwapBuffers(this->m_window);
        }
        void window::poll_events() {
            glfwPollEvents();
        }
    }
}