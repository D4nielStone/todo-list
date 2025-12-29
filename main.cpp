#include <bgui.hpp>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class TaskDatabase {
private:
    sqlite3* db;
    sqlite3_stmt* add_stmt;
    sqlite3_stmt* update_stmt;
    sqlite3_stmt* delete_stmt;
    
public:
    TaskDatabase(const std::string& db_path) : add_stmt(nullptr), update_stmt(nullptr), delete_stmt(nullptr) {
        if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        }
        
        // Enable WAL mode for better performance
        sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
        sqlite3_exec(db, "PRAGMA synchronous=NORMAL;", nullptr, nullptr, nullptr);
        
        create_table();
        prepare_statements();
    }
    
    ~TaskDatabase() {
        sqlite3_finalize(add_stmt);
        sqlite3_finalize(update_stmt);
        sqlite3_finalize(delete_stmt);
        sqlite3_close(db);
    }
    
    void create_table() {
        const char* sql = 
            "CREATE TABLE IF NOT EXISTS tasks ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "title TEXT NOT NULL,"
            "completed INTEGER DEFAULT 0);";
        
        char* err_msg = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
            std::cerr << "SQL error: " << err_msg << std::endl;
            sqlite3_free(err_msg);
        }
    }
    
    void prepare_statements() {
        // Prepare statements once for reuse
        sqlite3_prepare_v2(db, "INSERT INTO tasks (title, completed) VALUES (?, 0);", -1, &add_stmt, nullptr);
        sqlite3_prepare_v2(db, "UPDATE tasks SET completed = ? WHERE id = ?;", -1, &update_stmt, nullptr);
        sqlite3_prepare_v2(db, "DELETE FROM tasks WHERE id = ?;", -1, &delete_stmt, nullptr);
    }
    
    int add_task(const std::string& title) {
        sqlite3_reset(add_stmt);
        sqlite3_bind_text(add_stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(add_stmt);
        return sqlite3_last_insert_rowid(db);
    }
    
    void update_task_status(int id, bool completed) {
        sqlite3_reset(update_stmt);
        sqlite3_bind_int(update_stmt, 1, completed ? 1 : 0);
        sqlite3_bind_int(update_stmt, 2, id);
        sqlite3_step(update_stmt);
    }
    
    void delete_task(int id) {
        sqlite3_reset(delete_stmt);
        sqlite3_bind_int(delete_stmt, 1, id);
        sqlite3_step(delete_stmt);
    }
    
    struct Task {
        int id;
        std::string title;
        bool completed;
    };
    
    std::vector<Task> get_all_tasks() {
        std::vector<Task> tasks;
        tasks.reserve(100); // Pre-allocate for better performance
        
        const char* sql = "SELECT id, title, completed FROM tasks;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                Task task;
                task.id = sqlite3_column_int(stmt, 0);
                task.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                task.completed = sqlite3_column_int(stmt, 2) == 1;
                tasks.push_back(std::move(task));
            }
            sqlite3_finalize(stmt);
        }
        return tasks;
    }
};

// Global database instance
std::unique_ptr<TaskDatabase> g_db;

// Cache task elements to avoid linear search
std::unordered_map<int, bgui::checkbox*> g_task_map;

void create_task_from_db(int task_id, const std::string& title, bool completed) {
    auto& task = bgui::get_layout().add<bgui::checkbox>(nullptr, completed, title, 0.35f);
    task.request_width(bgui::mode::match_parent);
    task.request_height(bgui::mode::wrap_content);
    task.set_alignment(bgui::alignment::start);
    
    auto& style = task.get_style();
    style.m_button_border_color[3] = 0.f;
    style.m_button_color[3] = 0.5f;
    
    task.get_label().set_margin(10, 10);
    task.get_label().get_material().set("text_color", bgui::color{0.1f, 0.1f, 0.1f, 1.f});
    
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
    
    close.set_padding(30, 0);
    close.request_height(bgui::mode::match_parent);
    
    auto& close_style = close.get_style();
    close_style.m_button_border_color[3] = 0.f;
    close_style.m_button_color[3] = 0.5f;
    
    close.set_cross_alignment(bgui::alignment::center);
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
    g_db = std::make_unique<TaskDatabase>("todo_list.db");
    
    // set up backends
    GLFWwindow* window = bgui::set_up_glfw(600, 400, "Todo List App");
    bgui::set_up_gl3();
    bgui::set_up_freetype();
    bgui::set_up();

    // The main page
    auto& root = bgui::set_layout<bgui::linear>(bgui::orientation::vertical);
    auto& top_div = root.add<bgui::linear>(bgui::orientation::vertical);
    top_div.request_width(bgui::mode::match_parent);
    top_div.request_height(bgui::mode::pixel, 100.f);
    top_div.set_cross_alignment(bgui::alignment::center);
    top_div.add<bgui::text>("Todo List", 0.6f);
    
    auto& ia = top_div.add<bgui::input_area>("", 0.35f, "Task Title");
    ia.request_width(bgui::mode::match_parent);
    
    auto& btn = ia.add<bgui::button>("Add", 0.4f, [&ia](){
        std::string title = ia.get_label().get_buffer();
        if (!title.empty()) {
            create_task(title);
            ia.get_label().set_buffer("");
        }
    });
    
    top_div.set_visible(true);
    auto new_style = bgui::light_style;
    new_style.m_text_color = {1.f, 1.f, 1.f, 1.f};
    new_style.m_button_color = {0.8f, 0.8f, 0.8f, 1.f};
    bgui::apply_style(new_style);
    
    auto& top_material = top_div.get_material();
    top_material.set("bg_color", bgui::color{1.f, 0.6f, 0.f, 1.f});
    top_material.set("bordered", false);
    
    btn.set_padding(10, 0);
    btn.get_material().set("bordered", false);
    
    auto& ia_material = ia.get_material();
    ia_material.set("bordered", false);
    ia_material.set("text_color", bgui::color{0.4f, 0.4f, 0.4f, 1.f});

    // Load existing tasks from database
    load_tasks_from_db();

    // Cache layout elements
    int frame_counter = 0;
    const int style_update_interval = 1; // Update styles every frame, can be increased if needed

    while(!bgui::should_close_glfw()) {
        bgui::glfw_update(bgui::get_context());
        bgui::on_update();
        
        // Only update alternating colors when needed
        if (frame_counter % style_update_interval == 0) {
            int a = 0;
            auto& elements = bgui::get_layout().get_elements();
            for(auto& e : elements) {
                auto& style = e->get_style();
                style.m_cb_color[3] = (a % 2 == 0) ? 0.f : 1.f;
                style.m_cb_border_color[3] = 0.f;
                ++a;
            }
        }
        ++frame_counter;
        
        bgui::gl3_render(bgui::get_draw_data());
        bgui::swap_glfw();
    }
    
    bgui::shutdown_gl3();
    bgui::shutdown_glfw();
    
    return 0;
}