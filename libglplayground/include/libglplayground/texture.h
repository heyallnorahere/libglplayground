#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        class texture : public ref_counted {
        public:
            struct settings {
                settings() {
                    memset(this, 0, sizeof(settings)); // quick and dirty
                }
                GLenum target;
                GLenum min_filter;
                GLenum mag_filter;
                GLenum wrap_s;
                GLenum wrap_t;
                GLenum format;
            };
            texture(const std::vector<uint8_t>& data, int32_t width, int32_t height, int32_t channels, const settings& s = settings());
            ~texture();
            void bind(uint32_t slot);
            GLuint get();
            static ref<texture> from_file(const std::string& path);
        private:
            GLuint m_id;
            GLenum m_target;
        };
    }
}