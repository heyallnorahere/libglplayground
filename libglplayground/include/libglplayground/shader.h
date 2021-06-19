#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        struct shader_source {
            std::string vertex, fragment, geometry; // will add more later
        };
        class shader : public ref_counted {
        public:
            shader(const shader_source& source);
            ~shader();
            void bind();
            void unbind();
            GLuint get();
        private:
            bool m_geometry;
            GLuint m_id;
        };
    }
}