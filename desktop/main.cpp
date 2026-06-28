#include "api/task.hpp"

int main() {
    // set up backends
    GLFWwindow* window = bgui::set_up_glfw(600, 400, "Todo List App");
    bgui::set_up_gl3();
    bgui::set_up_freetype();
    bgui::set_up();

    auto& root = bgui::set_layout<bgui::linear>(bgui::orientation::vertical);
    auto& header = root.add<bgui::linear>(bgui::orientation::horizontal);
    header.recives_input(true);
    header.add<bgui::text>("Todo List App", 0.4f);
    header.style.layout.require_mode(bgui::mode::match_parent, bgui::mode::wrap_content);
    header.style.visual.background.normal = {0.06f, 0.06f, 0.06f, 1.f};

    tlb::get_instance().setup_interface();

    // window decoration
    bgui::set_glfw_window_decoration(false);
    bgui::style_manager::get_instance().get_global().visual.background.normal = {0.f, 0.f, 0.f, 0.f};
    bgui::get_layout().style.visual.background.normal = {0.1f, 0.1f, 0.1f, 1.f};
    bgui::get_layout().style.visual.visible = true;
    bgui::get_layout().style.visual.border_radius = 10;

    bgui::get_context().m_refresh_func = [&]()
    {
        auto& io = bgui::get_context();

        bgui::glfw_update(io);

        if (header.is_drag().x != 0 && header.is_drag().y != 0)
        {
            auto& drag = io.m_draginfo;

            bgui::set_glfw_window_position(
                (int)(drag.mouse_screen.x - drag.offset.x),
                (int)(drag.mouse_screen.y - drag.offset.y)
            );
        }
        header.set_drag({0,0});

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