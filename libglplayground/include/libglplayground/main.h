#pragma once
#ifdef LIBGLPLAYGROUND_DEFINE_MAIN
#ifdef APP_NAMESPACE
namespace APP_NAMESPACE {
#endif
extern ::libplayground::gl::ref<::libplayground::gl::application> get_application_instance();
#ifdef APP_NAMESPACE
}
#else
#define APP_NAMESPACE
#endif
int main(int argc, const char* argv[]) {
    using namespace ::libplayground::gl;
    ref<application> app = APP_NAMESPACE::get_application_instance();
    try {
        app->run();
    } catch (std::exception e) {
        spdlog::error(e.what());
        return 1;
    }
    return 0;
}
#endif