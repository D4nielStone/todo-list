#include <iostream>
#include "task_data_base.hpp"

task_database::task_database(const std::string& db_path)
: add_stmt(nullptr),
    update_stmt(nullptr),
    delete_stmt(nullptr) {
    if (sqlite3_open(
            db_path.c_str(),
            &db
        ) != SQLITE_OK) {
        std::cerr
            << "Can't open database: "
            << sqlite3_errmsg(db)
            << std::endl;
    }
    sqlite3_exec(
        db,
        "PRAGMA journal_mode=WAL;",
        nullptr,
        nullptr,
        nullptr
    );
    sqlite3_exec(
        db,
        "PRAGMA synchronous=NORMAL;",
        nullptr,
        nullptr,
        nullptr
    );
    create_table();
    prepare_statements();
}
task_database::~task_database() {

    sqlite3_finalize(add_stmt);

    sqlite3_finalize(update_stmt);

    sqlite3_finalize(delete_stmt);

    sqlite3_close(db);
}

void task_database::create_table() {
    const char* sql =

        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "completed INTEGER DEFAULT 0);";

    char* err_msg = nullptr;
    if (
        sqlite3_exec(
            db,
            sql,
            nullptr,
            nullptr,
            &err_msg
        ) != SQLITE_OK
    ) {

        std::cerr
            << "SQL error: "
            << err_msg
            << std::endl;

        sqlite3_free(err_msg);
    }
}

void task_database::prepare_statements() {
    sqlite3_prepare_v2(
        db,
        "INSERT INTO tasks "
        "(title, completed) "
        "VALUES (?, 0);",
        -1,
        &add_stmt,
        nullptr
    );
    sqlite3_prepare_v2(
        db,

        "UPDATE tasks "
        "SET completed = ? "
        "WHERE id = ?;",

        -1,
        &update_stmt,
        nullptr
    );
    sqlite3_prepare_v2(
        db,

        "DELETE FROM tasks "
        "WHERE id = ?;",

        -1,
        &delete_stmt,
        nullptr
    );
}

int task_database::add_task(
    const std::string& title
) {
    sqlite3_reset(add_stmt);
    sqlite3_bind_text(
        add_stmt,
        1,
        title.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    sqlite3_step(add_stmt);
    return sqlite3_last_insert_rowid(db);
}

void task_database::update_task_status(
    int id,
    bool completed
) {
    sqlite3_reset(update_stmt);
    sqlite3_bind_int(
        update_stmt,
        1,
        completed ? 1 : 0
    );
    sqlite3_bind_int(
        update_stmt,
        2,
        id
    );
    sqlite3_step(update_stmt);
}

void task_database::delete_task(int id) {
    sqlite3_reset(delete_stmt);

    sqlite3_bind_int(
        delete_stmt,
        1,
        id
    );

    sqlite3_step(delete_stmt);
}

std::vector<task> task_database::get_all_tasks() {

    std::vector<task> tasks;

    const char* sql =

        "SELECT id, title, completed "
        "FROM tasks;";

    sqlite3_stmt* stmt;

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        ) == SQLITE_OK
    ) {

        while (
            sqlite3_step(stmt)
            == SQLITE_ROW
        ) {

            task t;

            t.id =
                sqlite3_column_int(
                    stmt,
                    0
                );

            t.title =
                reinterpret_cast<
                    const char*
                >(
                    sqlite3_column_text(
                        stmt,
                        1
                    )
                );

            t.completed =
                sqlite3_column_int(
                    stmt,
                    2
                ) == 1;

            tasks.push_back(
                std::move(t)
            );
        }

        sqlite3_finalize(stmt);
    }

    return tasks;
}