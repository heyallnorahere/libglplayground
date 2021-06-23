#include "libglppch.h"
#include "texture.h"
#ifdef SHARED_ASSIMP
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>
namespace libplayground {
    namespace gl {
        texture::texture(const std::vector<uint8_t>& data, int32_t width, int32_t height, int32_t channels, const settings& s) {
            glGenTextures(1, &this->m_id);
            this->m_target = s.target ? s.target : GL_TEXTURE_2D;
            glBindTexture(this->m_target, this->m_id);
#define TEXPARAMETERI(name, field, default_value) glTexParameteri(this->m_target, name, s.field ? s.field : default_value)
            TEXPARAMETERI(GL_TEXTURE_MIN_FILTER, min_filter, GL_LINEAR);
            TEXPARAMETERI(GL_TEXTURE_MAG_FILTER, mag_filter, GL_LINEAR);
            TEXPARAMETERI(GL_TEXTURE_WRAP_S, wrap_s, GL_CLAMP_TO_EDGE);
            TEXPARAMETERI(GL_TEXTURE_WRAP_T, wrap_t, GL_CLAMP_TO_EDGE);
#undef TEXPARAMETERI
            GLint internal_format;
            switch (channels) {
            case 1:
                internal_format = GL_R8;
                break;
            case 2:
                internal_format = GL_RG;
                break;
            case 3:
                internal_format = GL_RGB;
                break;
            case 4:
                internal_format = GL_RGBA;
                break;
            }
            GLenum format = s.format ? s.format : (GLenum)internal_format;
            glTexImage2D(this->m_target, 0, internal_format, (GLsizei)width, (GLsizei)height, 0, format, GL_UNSIGNED_BYTE, data.data());
            glGenerateMipmap(this->m_target);
        }
        texture::~texture() {
            glDeleteTextures(1, &this->m_id);
        }
        void texture::bind(uint32_t slot) {
            glActiveTexture(GL_TEXTURE0 + (GLenum)slot);
            glBindTexture(this->m_target, this->m_id);
        }
        GLuint texture::get() {
            return this->m_id;
        }
        ref<texture> texture::from_file(const std::string& path) {
            int32_t width, height, channels;
            uint8_t* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
            ref<texture> tex;
            if (data) {
                std::vector<uint8_t> image_data(static_cast<size_t>(width * height * channels));
                memcpy(image_data.data(), data, (size_t)width * (size_t)height * (size_t)channels * sizeof(uint8_t)); // todo: not this
                settings s;
                tex = ref<texture>::create(image_data, width, height, channels, s);
            } else {
                throw std::runtime_error("Could not load image: " + path);
            }
            return tex;
        }
    }
}