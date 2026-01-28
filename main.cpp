#include <bgui.hpp>
#include "task.hpp"

// Global database instance
std::unique_ptr<task_data_base> g_db;

// Cache task elements to avoid linear search
std::unordered_map<int, bgui::checkbox*> g_task_map;

void create_task_from_db(int task_id, const std::string& title, bool completed) {
    auto& task = bgui::get_layout().add<bgui::checkbox>(nullptr, completed, title, 0.35f);

    // Cache the task pointer
    g_task_map[task_id] = &task;
    
    // Update database when checkbox state changes
    task.set_on_change([task_id](bool checked) {
        if (g_db) {
            g_db->update_task_status(task_id, checked);
        }
    });

    auto& close = task.add<bgui::button>("Remove", 0.4f, [&task, task_id](){
        if (g_db) {
            g_db->delete_task(task_id);
        }
        g_task_map.erase(task_id);
        bgui::get_layout().remove(&task);
    });
    close.style.layout.require_mode(bgui::mode::wrap_content, bgui::mode::wrap_content);
    
}

void create_task(const std::string& title) {
    if (title.empty()) return;
    
    int task_id = g_db->add_task(title);
    if (task_id != -1) {
        create_task_from_db(task_id, title, false);
    }
}

void load_tasks_from_db() {
    auto tasks = g_db->get_all_tasks();
    g_task_map.reserve(tasks.size());
    
    for (const auto& task : tasks) {
        create_task_from_db(task.id, task.title, task.completed);
    }
}

int main() {
    // Initialize database
    g_db = std::make_unique<task_data_base>(".todo_list.db");
    
    // set up backends
    GLFWwindow* window = bgui::set_up_glfw(600, 400, "Todo List App");
    bgui::set_up_gl3();
    bgui::set_up_freetype();
    bgui::set_up();

    // The main page
    auto& root = bgui::set_layout<bgui::linear>(bgui::orientation::vertical);
    root.style.layout.require_mode(bgui::mode::match_parent, bgui::mode::match_parent);
    auto& top_div = root.add<bgui::linear>(bgui::orientation::vertical);
    top_div.style = {
        .layout = {
            .size_mode = std::make_optional<bgui::vec<2Ul, bgui::mode>>({bgui::mode::match_parent, bgui::mode::wrap_content})
        },
        .visual = {
            .visible = true
        }
    };
    top_div.add<bgui::text>("Todo List", 0.6f).style.layout.require_mode(bgui::mode::stretch, bgui::mode::wrap_content);
    auto& ia = top_div.add<bgui::input_area>("", 0.4, "Task Title");
    auto& btn = ia.add<bgui::button>("Add", 0.4f, [&ia](){
        std::string title = ia.get_buffer();
        if (!title.empty()) {
            create_task(title);
        } else {
            create_task("Unnamed task");
        }
    });
    btn.style.layout.require_mode(bgui::mode::wrap_content, bgui::mode::wrap_content);

    // Load existing tasks from database
    load_tasks_from_db();

    while(!bgui::should_close_glfw()) {
        bgui::glfw_update(bgui::get_context());
        bgui::on_update();
        bgui::gl3_render(bgui::get_draw_data());
        bgui::swap_glfw();
    }
    
    bgui::shutdown_gl3();
    bgui::shutdown_glfw();
    bgui::shutdown_freetype();
    bgui::shutdown_lib();
    
    return 0;
}