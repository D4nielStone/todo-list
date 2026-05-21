cmake_minimum_required(VERSION 3.16)

project(server)

add_executable(server
    server.cpp
    database/task_data_base.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/backend
)
target_include_directories(server PRIVATE
    ${CMAKE_SOURCE_DIR}/external/crow/include
)

find_package(SQLite3 REQUIRED)

target_link_libraries(server PRIVATE
    SQLite::SQLite3
    pthread
    cpr::cpr
)