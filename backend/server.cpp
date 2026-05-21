#include "crow.h"
#include "crow/middlewares/cors.h"
#include "database/task_data_base.hpp"

int main() {

    crow::App<crow::CORSHandler> app;

    task_database db(
        "tasks.db"
    );

    app.get_middleware<
        crow::CORSHandler
    >()

    .global()

    .headers("Content-Type")

    .methods(
        "GET"_method,
        "POST"_method,
        "PUT"_method,
        "DELETE"_method
    )

    .origin("*");

    CROW_ROUTE(app, "/tasks")
    ([&]() {

        auto tasks =
            db.get_all_tasks();

        crow::json::wvalue res;

        for (
            size_t i = 0;
            i < tasks.size();
            i++
        ) {

            res[i]["id"] =
                tasks[i].id;

            res[i]["title"] =
                tasks[i].title;

            res[i]["completed"] =
                tasks[i].completed;
        }

        return res;
    });

    app.port(3000)
       .multithreaded()
       .run();
}