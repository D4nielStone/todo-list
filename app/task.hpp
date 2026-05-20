#pragma once
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class task_data_base {
private:
    sqlite3* db;
    sqlite3_stmt* add_stmt;
    sqlite3_stmt* update_stmt;
    sqlite3_stmt* delete_stmt;
    
public:
    task_data_base(const std::string& db_path) : add_stmt(nullptr), update_stmt(nullptr), delete_stmt(nullptr) {
        if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        }
        
        // Enable WAL mode for better performance
        sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
        sqlite3_exec(db, "PRAGMA synchronous=NORMAL;", nullptr, nullptr, nullptr);
        
        create_table();
        prepare_statements();
    }
    
    ~task_data_base() {
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
    
    struct task {
        int id;
        std::string title;
        bool completed;
    };
    
    std::vector<task> get_all_tasks() {
        std::vector<task> tasks;
        tasks.reserve(100); // Pre-allocate for better performance
        
        const char* sql = "SELECT id, title, completed FROM tasks;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                task task;
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