#pragma once
#include "shader.h"
namespace libplayground {
    namespace gl {
        // should be allocated on the stack
        class shader_factory {
        public:
            ref<shader> multiple_files(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path = "");
        };
    }
}