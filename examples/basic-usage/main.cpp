#define LIBGLPLAYGROUND_DEFINE_MAIN
#define APP_NAMESPACE basic_usage
#include <libglplayground.h>
using namespace libplayground::gl;
namespace basic_usage {
    class basic_usage_app : public application {
    public:
        basic_usage_app() : application("basic-usage") { }
    protected:
        virtual void load_content() override {
            // todo: load
        }
        virtual void unload_content() override {
            // todo: load
        }
    };
    ref<application> get_application_instance() {
        return ref<basic_usage_app>::create();
    }
}