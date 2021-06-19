#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        struct vertex_attribute {
            GLenum type;
            size_t elements, stride, offset;
            bool normalized;
        };
        class vertex_array_object : public ref_counted {
        public:
            vertex_array_object(const std::vector<vertex_attribute>& attributes);
            ~vertex_array_object();
            void bind();
            void unbind();
            GLuint get();
        private:
            GLuint m_id;
        };
    }
}