#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        class element_buffer_object : public ref_counted {
        public:
            element_buffer_object(const std::vector<uint32_t>& data);
            ~element_buffer_object();
            void bind();
            void unbind();
            void draw(GLenum mode);
            GLuint get();
        private:
            GLuint m_id;
            size_t m_index_count;
        };
    }
}