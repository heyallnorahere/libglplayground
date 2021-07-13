#pragma once
#ifndef LIBGLPLAYGROUND_INCLUDED
#error You must include libglplayground.h before this header!
#endif
namespace common {
    class player_behavior : public ::libplayground::gl::script {
    public:
        static float& sensitivity();
        static float& movement_speed();
        player_behavior();
        virtual void update() override;
    private:
        void calculate_camera_direction();
        glm::vec2 m_last_mouse;
    };
}