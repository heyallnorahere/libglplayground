#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        class window;
        class renderer;
        class scene;
        class application : public ref_counted {
        public:
            static ref<application> get_running_application();
            application(const std::string& title, int32_t width = 800, int32_t height = 600, bool mesa_context = false, int32_t major_opengl_version = 3, int32_t minor_opengl_version = 3);
            void run();
            void quit();
        protected:
            virtual void load_content();
            virtual void unload_content();
            virtual void update();
            virtual void render();
            std::string m_title;
            ref<window> m_window;
            ref<renderer> m_renderer;
            ref<scene> m_scene;
            bool m_terminated;
        };
    }
}