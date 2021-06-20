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
        ref<shader> shader_factory::single_file(const std::string& path) {
            std::string source = read_file(path);
            shader_source shader_data;
            std::string line;
            std::stringstream stream;
            stream << source;
            std::string preprocessor_definition = "#shader ";
            std::string current_shader_name;
            std::map<std::string, std::stringstream> sources;
            while (std::getline(stream, line)) {
                size_t location = line.find(preprocessor_definition);
                if (location != std::string::npos) {
                    size_t shader_name_pos = location + preprocessor_definition.length();
                    current_shader_name = line.substr(shader_name_pos);
                } else if (!current_shader_name.empty()) {
                    if (sources.find(current_shader_name) == sources.end()) {
                        sources.insert({ current_shader_name, std::stringstream() });
                    }
                    sources[current_shader_name] << line << "\n";
                }
            }
            if (sources.find("vertex") != sources.end()) {
                shader_data.vertex = sources["vertex"].str();
            }
            if (sources.find("fragment") != sources.end()) {
                shader_data.fragment = sources["fragment"].str();
            }
            if (sources.find("geometry") != sources.end()) {
                shader_data.geometry = sources["geometry"].str();
            }
            return ref<shader>::create(shader_data);
        }
    }
}