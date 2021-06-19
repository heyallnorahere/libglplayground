#include "libglppch.h"
#include "shader_factory.h"
namespace libplayground {
    namespace gl {
        static std::string read_file(const std::string& path) {
            std::ifstream file(path);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open file " + path);
            }
            std::stringstream content;
            std::string line;
            while (std::getline(file, line)) {
                content << line << "\n";
            }
            file.close();
            return content.str();
        }
        ref<shader> shader_factory::multiple_files(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path) {
            shader_source source;
            source.vertex = read_file(vertex_path);
            source.fragment = read_file(fragment_path);
            if (!geometry_path.empty()) {
                source.geometry = read_file(geometry_path);
            }
            return ref<shader>::create(source);
        }
    }
}