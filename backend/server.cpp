#include "crow.h"
#include "crow/middlewares/cors.h"

#include "database/task_data_base.hpp"

#include <filesystem>
#include <iostream>

int main() {

    std::cout
        << "Current path: "
        << std::filesystem::current_path()
        << std::endl;

    crow::App<crow::CORSHandler> app;

    task_database db(
        ".tasks.db"
    );

    // CORS
    app.get_middleware<crow::CORSHandler>()

        .global()

        .headers(
            "Content-Type"
        )

        .methods(
            crow::HTTPMethod::GET,
            crow::HTTPMethod::POST,
            crow::HTTPMethod::PUT,
            crow::HTTPMethod::DELETE
        )

        .origin("*");

    // GET /tasks
    CROW_ROUTE(app, "/tasks")
    .methods(crow::HTTPMethod::GET)

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

        return crow::response(res);
    });

    // POST /tasks
    CROW_ROUTE(app, "/tasks")
    .methods(crow::HTTPMethod::POST)

    ([&](const crow::request& req) {

        auto body =
            crow::json::load(
                req.body
            );

        if (!body) {

            return crow::response(
                400,
                "invalid json"
            );
        }

        if (!body.has("title")) {

            return crow::response(
                400,
                "missing title"
            );
        }

        std::string title =
            body["title"].s();

        int id =
            db.add_task(title);

        crow::json::wvalue res;

        res["id"] = id;
        res["title"] = title;
        res["completed"] = false;

        return crow::response(
            201,
            res
        );
    });

    // PUT /tasks/<id>
    CROW_ROUTE(app, "/tasks/<int>")
    .methods(crow::HTTPMethod::PUT)

    ([&](const crow::request& req,
         int id) {

        auto body =
            crow::json::load(
                req.body
            );

        if (!body) {

            return crow::response(
                400,
                "invalid json"
            );
        }

        if (!body.has("completed")) {

            return crow::response(
                400,
                "missing completed"
            );
        }

        bool completed =
            body["completed"].b();

        db.update_task_status(
            id,
            completed
        );

        return crow::response(200);
    });

    // DELETE /tasks/<id>
    CROW_ROUTE(app, "/tasks/<int>")
    .methods(crow::HTTPMethod::DELETE)

    ([&](int id) {

        db.delete_task(id);

        return crow::response(200);
    });

    // root
    CROW_ROUTE(app, "/")
    ([]() {

        return "task api running";
    });

    std::cout
        << "Server running at:"
        << std::endl;

    std::cout
        << "http://localhost:3000"
        << std::endl;

    app.port(3000)
       .multithreaded()
       .run();
}