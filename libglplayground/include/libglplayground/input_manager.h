#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        class window;
        class application;
        enum class key {
            Q, W, E, R, T, Y, U, I, O, P,
            A, S, D, F, G, H, J, K, L,
            Z, X, C, V, B, N, M
            // todo: add more keys
        };
        // The key was just pressed down.
        constexpr uint8_t key_down = 0b001;
        // The key has been held for a frame or more.
        constexpr uint8_t key_held = 0b010;
        // The key was just released.
        constexpr uint8_t key_released = 0b100;
        class input_manager : public ref_counted {
        public:
            static ref<input_manager> get();
            input_manager(const input_manager&) = delete;
            input_manager& operator=(input_manager&) = delete;
            uint8_t get_key(key key_enum);
            glm::vec2 get_mouse();
            void disable_mouse();
            void enable_mouse();
        private:
            input_manager(ref<window> window);
            void update();
            static void create(ref<window> window);
            static void cursor_pos_callback(GLFWwindow* window, double x, double y);
            ref<window> m_window;
            glm::vec2 m_mouse_offset;
            friend class application;
        };
    }
}