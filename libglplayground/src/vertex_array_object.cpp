#include "libglppch.h"
#include "vertex_array_object.h"
namespace libplayground {
    namespace gl {
        vertex_array_object::vertex_array_object(const std::vector<vertex_attribute>& attributes) {
            glGenVertexArrays(1, &this->m_id);
            for (size_t i = 0; i < attributes.size(); i++) {
                const auto& attrib = attributes[i];
                glVertexAttribPointer((GLuint)i, (GLint)attrib.elements, attrib.type, attrib.normalized, (GLsizei)attrib.stride, (void*)attrib.offset);
                glEnableVertexAttribArray((GLuint)i);
            }
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