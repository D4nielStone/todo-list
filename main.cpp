#include <bgui.hpp>

int main() {
    // set up backends
    GLFWwindow* window = bgui::set_up_glfw(600, 400, "Todo List App");
    bgui::set_up_gl3();
    bgui::set_up_freetype();

    while(!bgui::should_close_glfw()) {
        bgui::glfw_update(bgui::get_context());
        bgui::gl3_render(bgui::get_draw_data());
        bgui::swap_glfw();
    }
    bgui::shutdown_gl3();
    bgui::shutdown_glfw();
    return 0;
}