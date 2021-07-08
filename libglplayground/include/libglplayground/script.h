#pragma once
#include "ref.h"
namespace libplayground {
    namespace gl {
        namespace components {
            struct script_component;
        }
        class script : public ref_counted {
        public:
            virtual void update() {
            }
        protected:
            entity m_entity;
            friend struct components::script_component;
        };
    }
}