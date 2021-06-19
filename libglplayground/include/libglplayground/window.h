#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
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
            static void poll_events();
        private:
            GLFWwindow* m_window;
            std::string m_title;
            int32_t m_width, m_height;
        };
    }
}