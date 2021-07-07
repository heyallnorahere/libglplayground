#include "libglppch.h"
#include "shader.h"
namespace libplayground {
    namespace gl {
        extern bool _context_destroyed_;
        static GLuint create_shader(const std::string& source, GLenum type) {
            GLuint shader = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);
            GLint succeeded;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &succeeded);
            if (!succeeded) {
                GLchar info_log[512];
                glGetShaderInfoLog(shader, 512, nullptr, info_log);
                std::string shader_name;
                switch (type) {
                case GL_VERTEX_SHADER:
                    shader_name = "Vertex";
                    break;
                case GL_FRAGMENT_SHADER:
                    shader_name = "Fragment";
                    break;
                case GL_GEOMETRY_SHADER:
                    shader_name = "Geometry";
                    break;
                default:
                    shader_name = "Unimplemented";
                    break;
                }
                throw std::runtime_error(shader_name + "shader failed to compile: " + info_log);
            }
            return shader;
        }
        shader::shader(const shader_source& source) {
            if (source.vertex.empty()) {
                throw std::runtime_error("Vertex shader source cannot be empty!");
            }
            if (source.fragment.empty()) {
                throw std::runtime_error("Fragment shader source cannot be empty!");
            }
            std::vector<GLuint> shaders;
            shaders.push_back(create_shader(source.vertex, GL_VERTEX_SHADER));
            shaders.push_back(create_shader(source.fragment, GL_FRAGMENT_SHADER));
            if (!source.geometry.empty()) {
                shaders.push_back(create_shader(source.geometry, GL_GEOMETRY_SHADER));
            }
            this->m_id = glCreateProgram();
            for (GLuint shader : shaders) {
                glAttachShader(this->m_id, shader);
            }
            glLinkProgram(this->m_id);
            GLint succeeded;
            glGetProgramiv(this->m_id, GL_LINK_STATUS, &succeeded);
            if (!succeeded) {
                GLchar info_log[512];
                glGetShaderInfoLog(this->m_id, 512, nullptr, info_log);
                throw std::runtime_error("Program failed to link: " + std::string(info_log));
            }
            for (GLuint shader : shaders) {
                glDeleteShader(shader);
            }
        }
        shader::~shader() {
            if (!_context_destroyed_)
                glDeleteProgram(this->m_id);
        }
        void shader::bind() {
            glUseProgram(this->m_id);
        }
        void shader::unbind() {
            glUseProgram(0);
        }
        GLuint shader::get() {
            return this->m_id;
        }
        void shader::uniform_int(const std::string& name, GLint value) {
            glUniform1i(this->get_uniform_location(name), value);
        }
        void shader::uniform_uint(const std::string& name, GLuint value) {
            glUniform1ui(this->get_uniform_location(name), value);
        }
        void shader::uniform_float(const std::string& name, GLfloat value) {
            glUniform1f(this->get_uniform_location(name), value);
        }
        void shader::uniform_vec2(const std::string& name, const glm::vec2& value) {
            glUniform2f(this->get_uniform_location(name), value.x, value.y);
        }
        void shader::uniform_vec3(const std::string& name, const glm::vec3& value) {
            glUniform3f(this->get_uniform_location(name), value.x, value.y, value.z);
        }
        void shader::uniform_vec4(const std::string& name, const glm::vec4& value) {
            glUniform4f(this->get_uniform_location(name), value.x, value.y, value.z, value.w);
        }
        void shader::uniform_mat4(const std::string& name, const glm::mat4& value, bool transpose) {
            glUniformMatrix4fv(this->get_uniform_location(name), 1, transpose, glm::value_ptr(value));
        }
        GLint shader::get_uniform_location(const std::string& name) {
            return glGetUniformLocation(this->m_id, name.c_str());
        }
    }
}