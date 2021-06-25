#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        class shader;
        class shader_library {
        public:
            shader_library(const shader_library&) = delete;
            shader_library& operator=(const shader_library&) = delete;
            ref<shader>& operator[](const std::string& name) {
                return this->m_shaders[name];
            }
            void add(const std::string& name, ref<shader> shader) {
                this->m_shaders.insert({ name, shader });
            }
            auto find(const std::string& name) {
                return this->m_shaders.find(name);
            }
            auto begin() {
                return this->m_shaders.begin();
            }
            auto end() {
                return this->m_shaders.end();
            }
            static shader_library& get() {
                static shader_library instance;
                return instance;
            }
        private:
            shader_library() { }
            std::unordered_map<std::string, ref<shader>> m_shaders;
        };
    }
}