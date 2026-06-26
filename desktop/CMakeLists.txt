cmake_minimum_required(VERSION 3.28)
project(TodoList)

include_directories(
    ${CMAKE_SOURCE_DIR}/desktop
)
add_executable(app ui/main.cpp)

target_link_libraries(app PRIVATE bgui cpr::cpr nlohmann_json::nlohmann_json)