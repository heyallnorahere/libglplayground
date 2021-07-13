#include <libglplayground.h>
#include "common/player_behavior.h"
using namespace libplayground::gl;
namespace common {
    float& player_behavior::sensitivity() {
        static float value = 0.1f;
        return value;
    }
    float& player_behavior::movement_speed() {
        static float value = 0.05f;
        return value;
    }
    player_behavior::player_behavior() {
        this->m_last_mouse = input_manager::get()->get_mouse();
    }
    void player_behavior::update() {
        this->calculate_camera_direction();
        auto& transform = this->m_entity.get_component<components::transform_component>();
        auto& camera = this->m_entity.get_component<components::camera_component>();
        ref<input_manager> im = input_manager::get();
        // todo: sync with framerate
        constexpr float movement_speed = 0.05f;
        glm::vec3 unit_right = glm::normalize(glm::cross(camera.direction, camera.up));
        if (im->get_key(key::W) & key_held) {
            transform.translation += movement_speed * camera.direction;
        }
        if (im->get_key(key::S) & key_held) {
            transform.translation -= movement_speed * camera.direction;
        }
        if (im->get_key(key::A) & key_held) {
            transform.translation -= movement_speed * unit_right;
        }
        if (im->get_key(key::D) & key_held) {
            transform.translation += movement_speed * unit_right;
        }
        if (im->get_key(key::O) & key_down) {
            im->enable_mouse();
        }
        if (im->get_key(key::P) & key_down) {
            im->disable_mouse();
        }
        if (im->get_key(key::Q) & key_down) {
            application::get_running_application()->quit();
        }
    }
    void player_behavior::calculate_camera_direction() {
        auto& camera = this->m_entity.get_component<components::camera_component>();
        glm::vec2 mouse_position = input_manager::get()->get_mouse();
        glm::vec2 offset = (mouse_position - this->m_last_mouse) * glm::vec2(1.f, -1.f);
        this->m_last_mouse = mouse_position;
        offset *= sensitivity();
        glm::vec2 angle;
        angle.x = glm::degrees(asin(camera.direction.y));
        float factor = cos(glm::radians(angle.x));
        angle.y = glm::degrees(atan2(camera.direction.z / factor, camera.direction.x / factor));
        angle += glm::vec2(offset.y, offset.x);
        if (angle.x > 89.f) {
            angle.x = 89.f;
        }
        if (angle.x < -89.f) {
            angle.x = -89.f;
        }
        glm::vec3 direction;
        direction.x = cos(glm::radians(angle.y)) * cos(glm::radians(angle.x));
        direction.y = sin(glm::radians(angle.x));
        direction.z = sin(glm::radians(angle.y)) * cos(glm::radians(angle.x));
        camera.direction = glm::normalize(direction);
    }
}