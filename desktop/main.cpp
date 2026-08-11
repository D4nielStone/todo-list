#include "api/task.hpp"

int main() {
    // set up backends
    GLFWwindow* window = bgui::set_up_glfw(600, 400, "Todo List App");
    bgui::set_up_gl3();
    bgui::set_up_freetype();
    bgui::set_up();

    auto& root = bgui::set_layout<bgui::linear>(bgui::orientation::vertical);
    tlb::get_instance().setup_interface();
    bgui::get_context().m_refresh_func = [&]()
    {
        auto& io = bgui::get_context();

        bgui::glfw_update(io);

        bgui::on_update();

        bgui::gl3_clear();
        bgui::gl3_render(bgui::get_draw_data());
        bgui::swap_glfw();
    };
    
    bgui::glfw_main_loop();
    
    bgui::shutdown_gl3();
    bgui::shutdown_glfw();
    bgui::shutdown_freetype();
    bgui::shutdown_lib();
    
    return 0;
}