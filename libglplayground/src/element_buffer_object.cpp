#include "libglppch.h"
#include "element_buffer_object.h"
namespace libplayground {
    namespace gl {
        element_buffer_object::element_buffer_object(const std::vector<uint32_t>& data) {
            glGenBuffers(1, &this->m_id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint32_t), data.data(), GL_STATIC_DRAW); // for now
            this->m_index_count = data.size();
        }
        element_buffer_object::~element_buffer_object() {
            glDeleteBuffers(1, &this->m_id);
        }
        void element_buffer_object::bind() {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
        }
        void element_buffer_object::unbind() {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        void element_buffer_object::draw(GLenum mode) {
            this->bind();
            glDrawElements(mode, (GLsizei)this->m_index_count, GL_UNSIGNED_INT, nullptr);
        }
        GLuint element_buffer_object::get() {
            return this->m_id;
        }
    }
}