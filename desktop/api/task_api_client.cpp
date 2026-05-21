#include "task_api_client.hpp"

task_api_client::task_api_client(const std::string& url) : m_url(url) {}
std::vector<task> task_api_client::get_all_tasks() {

    auto response =
        cpr::Get(
            cpr::Url{
                m_url + "/tasks"
            }
        );

    std::vector<task> tasks;

    if (response.status_code == 200) {

        auto json =
            nlohmann::json::parse(
                response.text
            );

        for (auto& item : json) {

            tasks.push_back({
                item["id"],
                item["title"],
                item["completed"]
            });
        }
    }

    return tasks;
}

int task_api_client::add_task(const std::string& title) {

    auto response =
        cpr::Post(
            cpr::Url{
                m_url + "/tasks"
            },

            cpr::Header{
                {"Content-Type",
                    "application/json"}
            },

            cpr::Body{
                nlohmann::json({
                    {"title", title}
                }).dump()
            }
        );

    if (response.status_code == 201) {

        auto json =
            nlohmann::json::parse(
                response.text
            );

        return json["id"];
    }

    return -1;
}

void task_api_client::update_task_status(
    int id,
    bool completed
) {

    cpr::Put(
        cpr::Url{
            m_url + "/tasks/" +
            std::to_string(id)
        },

        cpr::Header{
            {"Content-Type",
                "application/json"}
        },

        cpr::Body{
            nlohmann::json({
                {"completed", completed}
            }).dump()
        }
    );
}

void task_api_client::delete_task(int id) {

    cpr::Delete(
        cpr::Url{
            m_url + "/tasks/" +
            std::to_string(id)
        }
    );
}