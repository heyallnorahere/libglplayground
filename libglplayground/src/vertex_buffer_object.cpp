#include "libglppch.h"
#include "vertex_buffer_object.h"
namespace libplayground {
    namespace gl {
        vertex_buffer_object::~vertex_buffer_object() {
            glDeleteBuffers(1, &this->m_id);
        }
        void vertex_buffer_object::bind() {
            glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
        }
        void vertex_buffer_object::unbind() {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        void vertex_buffer_object::draw(GLenum mode) {
            glDrawArrays(mode, 0, (GLsizei)this->m_vertex_count);
        }
        GLuint vertex_buffer_object::get() {
            return this->m_id;
        }
        void vertex_buffer_object::init(const void* data, size_t length) {
            glGenBuffers(1, &this->m_id);
            glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
            glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)length, data, GL_STATIC_DRAW); // for now
        }
    }
}