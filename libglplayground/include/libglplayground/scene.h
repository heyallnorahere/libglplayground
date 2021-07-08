#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        class renderer;
        class window;
        class entity;
        class scene : public ref_counted {
        public:
            entity create();
            void destroy(const entity& entity);
            void update();
            void render(ref<renderer> renderer, ref<window> window);
            entity get_primary_camera_entity();
            template<typename T> void on_component_added(entity& ent, T& component);
        private:
            entt::registry m_registry;
            friend class entity;
        };
        // entity methods (from entity.h)
        template<typename T, typename... Args> inline T& entity::add_component(Args&&... args) {
            if (this->has_component<T>()) {
                throw std::runtime_error("This entity already has a component of type: " + std::string(typeid(T).name()));
            }
            T& component = this->m_scene->m_registry.emplace<T>(this->m_handle, std::forward<Args>(args)...);
            this->m_scene->on_component_added(*this, component);
            return component;
        }
        template<typename T> inline T& entity::get_component() {
            if (!this->has_component<T>()) {
                throw std::runtime_error("This entity does not have a component of type: " + std::string(typeid(T).name()));
            }
            return this->m_scene->m_registry.get<T>(this->m_handle);
        }
        template<typename T> inline bool entity::has_component() {
            return this->m_scene->m_registry.all_of<T>(this->m_handle);
        }
        template<typename T> inline void entity::remove_component() {
            if (!this->has_component<T>()) {
                throw std::runtime_error("This entity does not have a component of type: " + std::string(typeid(T).name()));
            }
            this->m_scene->m_registry.remove<T>(this->m_handle);
        }
        template<typename T> inline void scene::on_component_added(entity& ent, T& component) {
            // no specific definition exists, so just return
        }
    }
}