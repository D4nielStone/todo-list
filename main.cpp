#include <bgui.hpp>
#include <iostream>

void create_task() {
    std::cout << "New task\n";
}

int main() {
    // set up backends
    GLFWwindow* window = bgui::set_up_glfw(600, 400, "Todo List App");
    bgui::set_up_gl3();
    bgui::set_up_freetype();

    // initialize the library
    bgui::set_up();

    // The main page
    auto& top_div = bgui::get_layout().add<bgui::linear>(bgui::orientation::vertical);
    top_div.request_width(bgui::mode::match_parent);
    top_div.request_height(bgui::mode::pixel, 100.f);
    top_div.set_alignment(bgui::alignment::center);
    top_div.set_cross_alignment(bgui::alignment::center);
    top_div.add<bgui::text>("Todo List", 0.5f);
    auto& ia = top_div.add<bgui::input_area>("", 0.5f, "Task Title");
    ia.request_width(bgui::mode::match_parent);
    auto& spacer = ia.add<bgui::element>();
    spacer.request_width(bgui::mode::stretch);
    spacer.set_visible(false);
    auto& btn = ia.add<bgui::button>("Add", 0.5, create_task);
    top_div.set_visible(true);
    auto new_style = bgui::light_style;
    new_style.m_text_color = {1, 1, 1, 1};
    new_style.m_button_color = {0.8, 0.8, 0.8, 1};
    bgui::apply_style(new_style);
    top_div.get_material().set("bg_color", bgui::color{0.5, 0.1, 0.1, 1});
    btn.get_material().set("bordered", false);
    btn.set_padding(20, 0);
    ia.get_material().set("border_radius", 0.f);
    btn.get_label().get_style().m_text_color = {0.3, 0.3, 0.3, 1};

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