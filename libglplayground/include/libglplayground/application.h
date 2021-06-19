#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        class application : public ref_counted {
        public:
            application(const std::string& title, int32_t width = 800, int32_t height = 600, int32_t major_opengl_version = 3, int32_t minor_opengl_version = 3);
            void run();
        protected:
            virtual void load_content() = 0;
            virtual void unload_content() = 0;
            std::string m_title;
        };
    }
}