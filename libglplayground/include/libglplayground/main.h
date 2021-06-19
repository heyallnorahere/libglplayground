#pragma once
#ifdef LIBGLPLAYGROUND_DEFINE_MAIN
#ifdef LIBGLPLAYGROUND_APP_NAMESPACE
namespace LIBGLPLAYGROUND_APP_NAMESPACE {
#endif
extern ::libplayground::gl::ref<::libplayground::gl::application> get_application_instance();
#ifdef LIBGLPLAYGROUND_APP_NAMESPACE
}
#else
#define LIBGLPLAYGROUND_APP_NAMESPACE
#endif
int main(int argc, const char* argv[]) {
    using namespace ::libplayground::gl;
    ref<application> app = LIBGLPLAYGROUND_APP_NAMESPACE::get_application_instance();
    try {
        app->run();
    } catch (std::exception e) {
        spdlog::error(e.what());
        return 1;
    }
    return 0;
}
#endif