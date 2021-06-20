#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        using window_callback = std::function<void(void*)>;
        enum class window_callback_trigger {
            on_resize,
            on_clear,
            on_present
        };
        struct window_resize_event_args {
            int32_t new_width, new_height;
        };
        class window : public ref_counted {
        public:
            window(const std::string& title, int32_t width, int32_t height, int32_t major_opengl_version, int32_t minor_opengl_version);
            ~window();
            GLFWwindow* get();
            std::string get_title();
            int32_t get_width();
            int32_t get_height();
            bool should_window_close();
            void clear();
            void swap_buffers();
            void add_callback(window_callback_trigger trigger, window_callback callback);
            void call_callback(window_callback_trigger event, void* args);
            static void poll_events();
        private:
            static void on_framebuffer_size(GLFWwindow* window, int32_t width, int32_t height);
            std::vector<std::pair<window_callback_trigger, window_callback>> m_callbacks;
            GLFWwindow* m_window;
            std::string m_title;
            int32_t m_width, m_height;
        };
    }
}