#pragma once
namespace libplayground {
    namespace gl {
        class scene;
        class entity {
        public:
            entity() = default;
            entity(entt::entity handle, scene* scene_) {
                this->m_handle = handle;
                this->m_scene = scene_;
            }
            entity(const entity&) = default;
            entity& operator=(const entity&) = default;
            // templated functions involving "scene" will be in scene.h
            template<typename T, typename... Args> T& add_component(Args&&... args);
            template<typename T> T& get_component();
            template<typename T> bool has_component();
            template<typename T> void remove_component();
            operator bool() const {
                return this->m_handle != entt::null;
            }
            operator entt::entity() const {
                return this->m_handle;
            }
            operator uint32_t() const {
                return (uint32_t)this->m_handle;
            }
            bool operator==(const entity& other) const {
                return this->m_handle == other.m_handle && this->m_scene == other.m_scene;
            }
            bool operator!=(const entity& other) const {
                return !(*this == other);
            }
        private:
            entt::entity m_handle = entt::null;
            scene* m_scene = nullptr;
        };
    }
}