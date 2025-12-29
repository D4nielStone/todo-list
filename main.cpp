#include <bgui.hpp>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>

class TaskDatabase {
private:
    sqlite3* db;
    
public:
    TaskDatabase(const std::string& db_path) {
        if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        }
        create_table();
    }
    
    ~TaskDatabase() {
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
    
    int add_task(const std::string& title) {
        const char* sql = "INSERT INTO tasks (title, completed) VALUES (?, 0);";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            return sqlite3_last_insert_rowid(db);
        }
        return -1;
    }
    
    void update_task_status(int id, bool completed) {
        const char* sql = "UPDATE tasks SET completed = ? WHERE id = ?;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, completed ? 1 : 0);
            sqlite3_bind_int(stmt, 2, id);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    void delete_task(int id) {
        const char* sql = "DELETE FROM tasks WHERE id = ?;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, id);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    struct Task {
        int id;
        std::string title;
        bool completed;
    };
    
    std::vector<Task> get_all_tasks() {
        std::vector<Task> tasks;
        const char* sql = "SELECT id, title, completed FROM tasks;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                Task task;
                task.id = sqlite3_column_int(stmt, 0);
                task.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                task.completed = sqlite3_column_int(stmt, 2) == 1;
                tasks.push_back(task);
            }
            sqlite3_finalize(stmt);
        }
        return tasks;
    }
};

// Global database instance
TaskDatabase* g_db = nullptr;

void create_task_from_db(int task_id, const std::string& title, bool completed) {
    auto& task = bgui::get_layout().add<bgui::checkbox>(nullptr, completed, title, 0.35);
    task.request_width(bgui::mode::match_parent);
    task.request_height(bgui::mode::wrap_content);
    task.set_alignment(bgui::alignment::start);
    task.get_style().m_button_border_color[3]=0.f;
    task.get_style().m_button_color[3]=0.5f;
    task.get_label().set_margin(10, 10);
    task.get_label().get_material().set("text_color", bgui::color{0.1, 0.1, 0.1, 1});
    
    // Store task_id in user data if bgui supports it, or use a map
    // For this example, we'll capture it in the lambda
    
    // Update database when checkbox state changes
    task.set_on_change([task_id](bool checked) {
        if (g_db) {
            g_db->update_task_status(task_id, checked);
        }
    });

    auto& close = task.add<bgui::button>("Remove", 0.4, [&task, task_id](){
        if (g_db) {
            g_db->delete_task(task_id);
        }
        bgui::get_layout().remove(&task);
    });
    close.set_padding(30, 0);
    close.request_height(bgui::mode::match_parent);
    close.get_style().m_button_border_color[3]=0.f;
    close.get_style().m_button_color[3]=0.5f;
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
    for (const auto& task : tasks) {
        create_task_from_db(task.id, task.title, task.completed);
    }
}

int main() {
    // Initialize database
    g_db = new TaskDatabase("todo_list.db");
    
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
    
    auto& btn = ia.add<bgui::button>("Add", 0.4, [&ia](){
        std::string title = ia.get_label().get_buffer();
        create_task(title);
        ia.get_label().set_buffer(""); // Clear input after adding
    });
    
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

    // Load existing tasks from database
    load_tasks_from_db();

    while(!bgui::should_close_glfw()) {
        bgui::glfw_update(bgui::get_context());
        bgui::on_update();
        
        int a = 0;
        for(auto& e : bgui::get_layout().get_elements()) {
            e->get_style().m_cb_color[3]=(a % 2 == 0)?0:1;
            e->get_style().m_cb_border_color[3]=0.f;
            a++;
        }
        
        bgui::gl3_render(bgui::get_draw_data());
        bgui::swap_glfw();
    }
    
    bgui::shutdown_gl3();
    bgui::shutdown_glfw();
    
    delete g_db;
    return 0;
}