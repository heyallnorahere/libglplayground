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

            // uniform functions
            void uniform_int(const std::string& name, GLint value);
            void uniform_uint(const std::string& name, GLuint value);
            void uniform_float(const std::string& name, GLfloat value);
            void uniform_vec2(const std::string& name, const glm::vec2& value);
            void uniform_vec3(const std::string& name, const glm::vec3& value);
            void uniform_vec4(const std::string& name, const glm::vec4& value);
            void uniform_mat4(const std::string& name, const glm::mat4& value, bool transpose = false);
        private:
            GLint get_uniform_location(const std::string& name);
            GLuint m_id;
        };
    }
}