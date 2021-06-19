#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        class vertex_buffer_object : public ref_counted {
        public:
            template<typename T> vertex_buffer_object(const std::vector<T>& data) {
                this->init(data.data(), data.size() * sizeof(T));
                this->m_vertex_count = data.size();
            }
            ~vertex_buffer_object();
            void bind();
            void unbind();
            void draw(GLenum mode);
            GLuint get();
        private:
            void init(const void* data, size_t length);
            size_t m_vertex_count;
            GLuint m_id;
        };
    }
}