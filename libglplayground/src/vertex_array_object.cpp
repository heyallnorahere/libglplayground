#include "libglppch.h"
#include "vertex_array_object.h"
namespace libplayground {
    namespace gl {
        vertex_array_object::vertex_array_object(const std::vector<vertex_attribute>& attributes) {
            glGenVertexArrays(1, &this->m_id);
            glBindVertexArray(this->m_id);
        }
        vertex_array_object::~vertex_array_object() {
            glDeleteVertexArrays(1, &this->m_id);
        }
        void vertex_array_object::bind() {
            glBindVertexArray(this->m_id);
        }
        void vertex_array_object::unbind() {
            glBindVertexArray(0);
        }
        GLuint vertex_array_object::get() {
            return this->m_id;
        }
    }
}