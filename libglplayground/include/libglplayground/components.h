#pragma once
#include "renderer.h"
namespace libplayground {
    namespace gl {
        namespace components {
            struct transform_component {
                glm::mat4 transform;
            };
            struct mesh_component {
                std::vector<vertex> vertices;
                std::vector<uint32_t> indices;
                std::vector<ref<texture>> textures;
            };
        }
    }
}