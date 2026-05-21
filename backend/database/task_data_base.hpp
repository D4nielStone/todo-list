#pragma once

#include <sqlite3.h>
#include <vector>
#include <string>
#include "shared/models/task.hpp"

class task_database {
private:

    sqlite3* db;

    sqlite3_stmt* add_stmt;
    sqlite3_stmt* update_stmt;
    sqlite3_stmt* delete_stmt;

public:
    task_database(
        const std::string& db_path
    );
    ~task_database();
public:
    int add_task(
        const std::string& title
    );
    void update_task_status(
        int id,
        bool completed
    );
    void delete_task(int id);
    std::vector<task>
    get_all_tasks();
private:
    void create_table();
    void prepare_statements();
};