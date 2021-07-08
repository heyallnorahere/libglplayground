#include "libglppch.h"
#include "window.h"
#include "input_manager.h"
namespace libplayground {
    namespace gl {
        static ref<input_manager> global_input_manager;
        ref<input_manager> input_manager::get() {
            return global_input_manager;
        }
        input_manager::input_manager(ref<window> window) {
            this->m_window = window;
            this->m_mouse_offset = glm::vec2(0.f);
            glfwSetCursorPosCallback(this->m_window->get(), cursor_pos_callback);
        }
        void input_manager::update() {
            // todo: query glfwGetKey
        }
        void input_manager::create(ref<window> window) {
            global_input_manager = ref<input_manager>(new input_manager(window));
        }
        void input_manager::cursor_pos_callback(GLFWwindow* window, double x, double y) {
            // todo: update the m_mouse_offset field
        }
        uint8_t input_manager::get_key(key key_enum) {
            // todo: determine the state of the passed key value
            return 0;
        }
        glm::vec2 input_manager::get_mouse() {
            return this->m_mouse_offset;
        }
        void input_manager::disable_mouse() {
            glfwSetInputMode(this->m_window->get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        void input_manager::enable_mouse() {
            glfwSetInputMode(this->m_window->get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}