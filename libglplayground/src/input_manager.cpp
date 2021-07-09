#include "libglppch.h"
#include "window.h"
#include "input_manager.h"
namespace libplayground {
    namespace gl {
        static ref<input_manager> global_input_manager;
        ref<input_manager> input_manager::get() {
            return global_input_manager;
        }
        uint8_t input_manager::get_key(key key_enum) {
            uint8_t value = 0;
            auto state = this->m_states[key_enum];
            if (state.current) {
                value |= key_held;
            }
            if (state.current && !state.last) {
                value |= key_down;
            }
            if (state.last && !state.current) {
                value |= key_released;
            }
            return value;
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
        input_manager::input_manager(ref<window> window) {
            this->m_window = window;
            this->m_mouse_offset = glm::vec2(0.f);
            glfwSetCursorPosCallback(this->m_window->get(), cursor_pos_callback);
            this->insert_pairs();
        }
        void input_manager::insert_pairs() {
#define K(key_name) this->m_states[key::key_name].key_code = GLFW_KEY_##key_name
            K(Q); K(W); K(E); K(R); K(T); K(Y); K(U); K(I); K(O); K(P);
            K(A); K(S); K(D); K(F); K(G); K(H); K(J); K(L);
            K(Z); K(X); K(C); K(V); K(B); K(N); K(M);
#undef K // idk i thought K was good enough for "key" dont sue me lol
        }
        void input_manager::update() {
            for (auto& pair : this->m_states) {
                auto& state = pair.second;
                state.reset();
                state.current = (glfwGetKey(this->m_window->get(), state.key_code) == GLFW_PRESS);
            }
        }
        void input_manager::create(ref<window> window) {
            global_input_manager = ref<input_manager>(new input_manager(window));
        }
        void input_manager::cursor_pos_callback(GLFWwindow* window, double x, double y) {
            global_input_manager->m_mouse_offset += glm::vec2((float)x, (float)y);
        }
    }
}