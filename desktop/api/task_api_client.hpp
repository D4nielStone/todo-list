#pragma once

#include <vector>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "shared/models/task.hpp"

class task_api_client {
private:

    std::string m_url;

public:

    task_api_client(
        const std::string& url =
        "http://localhost:3000"
    );

public:

    std::vector<task>
    get_all_tasks();

    int add_task(
        const std::string& title
    );

    void update_task_status(
        int id,
        bool completed
    );

    void delete_task(int id);
};