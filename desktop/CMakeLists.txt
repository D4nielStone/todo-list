cmake_minimum_required(VERSION 3.28)
project(TodoList)

include_directories(
    ${CMAKE_SOURCE_DIR}/desktop
)
add_executable(app ui/main.cpp api/task_api_client.cpp)
find_package(SQLite3 REQUIRED)

target_link_libraries(app PRIVATE SQLite::SQLite3 bgui cpr::cpr nlohmann_json::nlohmann_json)