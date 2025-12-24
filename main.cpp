#include <bgui.hpp>

int main() {
    // set up backends
    GLFWwindow* window = bgui::set_up_glfw(600, 400, "Todo List App");
    bgui::set_up_gl3();
    bgui::set_up_freetype();

    // initialize the library
    bgui::set_up();

    // The main page
    bgui::get_layout().add<bgui::text>("Todo List", 0.5f);
    bgui::apply_style(bgui::dark_style);

    while(!bgui::should_close_glfw()) {
        // update the context with glfw
        bgui::glfw_update(bgui::get_context());
        // update the layout
        bgui::update();
        // render with opengl3
        bgui::gl3_render(bgui::get_draw_data());
        bgui::swap_glfw();
    }
    bgui::shutdown_gl3();
    bgui::shutdown_glfw();
    return 0;
}