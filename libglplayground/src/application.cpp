#include "libglppch.h"
#include "window.h"
#include "renderer.h"
#include "entity.h"
#include "scene.h"
#include "application.h"
#include "components.h"
#include "input_manager.h"
#ifdef BUILT_IMGUI
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#endif
namespace libplayground {
    namespace gl {
        static void init_imgui(ref<window> window) {
#ifdef BUILT_IMGUI
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            ImGui::StyleColorsDark();
            ImGuiStyle& style = ImGui::GetStyle();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                style.WindowRounding = 0.f;
                style.Colors[ImGuiCol_WindowBg].w = 1.f;
            }
            ImGui_ImplGlfw_InitForOpenGL(window->get(), true);
            ImGui_ImplOpenGL3_Init("#version 330 core");
#endif
        }
        static void terminate_imgui() {
#ifdef BUILT_IMGUI
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
#endif
        }
        static void imgui_begin_frame() {
#ifdef BUILT_IMGUI
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
#endif
        }
        static void imgui_end_frame(ref<window> window) {
#ifdef BUILT_IMGUI
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize.x = (float)window->get_width();
            io.DisplaySize.y = (float)window->get_height();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(window->get());
            }
#endif
        }
        application::application(const std::string& title, int32_t width, int32_t height, bool mesa_context, int32_t major_opengl_version, int32_t minor_opengl_version) {
            this->m_title = title;
            this->m_window = ref<window>::create(title, width, height, mesa_context, major_opengl_version, minor_opengl_version);
            this->m_renderer = ref<renderer>::create();
            this->m_scene = ref<scene>::create();
            input_manager::create(this->m_window);
        }
        void application::run() {
            spdlog::info("Starting application " + this->m_title + "...");
            init_imgui(this->m_window);
            this->load_content();
            while (!this->m_window->should_window_close()) {
                input_manager::get()->update();
                this->update();
                this->m_scene->update();
                this->m_window->clear();
                this->m_renderer->reset();
                imgui_begin_frame();
                this->render();
                this->m_scene->render(this->m_renderer, this->m_window);
                this->m_renderer->render();
                imgui_end_frame(this->m_window);
                this->m_window->swap_buffers();
                this->m_window->poll_events();
            }
            spdlog::info("Shutting down...");
            this->unload_content();
            terminate_imgui();
        }
        void application::load_content() { }
        void application::unload_content() { }
        void application::update() { }
        void application::render() { }
    }
}