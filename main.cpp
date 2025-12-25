#include <bgui.hpp>
#include <iostream>

void create_task() {
    static int a = 0;
    a++;
    auto& task = bgui::get_layout().add<bgui::button>("Task " + std::to_string(a), 0.35, [](){});
    task.request_width(bgui::mode::match_parent);
    task.request_height(bgui::mode::wrap_content);
    task.set_alignment(bgui::alignment::start);
    task.get_label().set_margin(10, 10);
    task.get_label().get_material().set("text_color", bgui::color{0.1, 0.1, 0.1, 1});

    auto& spacer = task.add<bgui::element>();
    spacer.request_width(bgui::mode::stretch);
    spacer.set_visible(false);
    spacer.recives_input(false);

    auto& close = task.add<bgui::button>("Remove", 0.4, [&](){
        bgui::get_layout().remove(&task);
    });
    close.set_padding(30, 0);
    close.request_height(bgui::mode::match_parent);
    close.get_style().m_button_border_color[3]=0.f;
    close.get_style().m_button_color[3]=0.5f;
    close.set_cross_alignment(bgui::alignment::center);
}

int main() {
    // set up backends
    GLFWwindow* window = bgui::set_up_glfw(600, 400, "Todo List App");
    bgui::set_up_gl3();
    bgui::set_up_freetype();

    // initialize the library
    bgui::set_up();

    // The main page
    auto& root = bgui::set_layout<bgui::linear>(bgui::orientation::vertical);
    auto& top_div = root.add<bgui::linear>(bgui::orientation::vertical);
    top_div.request_width(bgui::mode::match_parent);
    top_div.request_height(bgui::mode::pixel, 100.f);
    //top_div.set_alignment(bgui::alignment::center);
    top_div.set_cross_alignment(bgui::alignment::center);
    top_div.add<bgui::text>("Todo List", 0.6f);
    auto& ia = top_div.add<bgui::input_area>("", 0.35f, "Task Title");
    ia.request_width(bgui::mode::match_parent);
    auto& spacer = ia.add<bgui::element>();
    spacer.request_width(bgui::mode::stretch);
    spacer.set_visible(false);
    auto& btn = ia.add<bgui::button>("Add", 0.4, create_task);
    top_div.set_visible(true);
    auto new_style = bgui::light_style;
    new_style.m_text_color = {1, 1, 1, 1};
    new_style.m_button_color = {0.8, 0.8, 0.8, 1};
    bgui::apply_style(new_style);
    top_div.get_material().set("bg_color", bgui::color{1, 0.6, 0, 1});
    btn.set_padding(10, 0);
    btn.get_material().set("bordered", false);
    ia.get_material().set("bordered", false);
    top_div.get_material().set("bordered", false);
    ia.get_material().set("text_color", bgui::color{0.4, 0.4, 0.4, 1});

    while(!bgui::should_close_glfw()) {
        // update the context with glfw
        bgui::glfw_update(bgui::get_context());
        // update the layout
        bgui::update();
        // tasks color change

        // if is pair, hide color
        int a = 0;
        for(auto& e : bgui::get_layout().get_elements()) {
            e->get_style().m_button_color[3]=(a % 2 == 0)?0:1;
            e->get_style().m_button_border_color[3]=0.f;
            a++;
        }
        // render with opengl3
        bgui::gl3_render(bgui::get_draw_data());
        bgui::swap_glfw();
    }
    bgui::shutdown_gl3();
    bgui::shutdown_glfw();
    return 0;
}