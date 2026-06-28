cmake_minimum_required(VERSION 3.28)
project(TodoListApp)

include_directories(
    ${CMAKE_SOURCE_DIR}/desktop
)
add_executable(app main.cpp)

target_link_libraries(app PRIVATE bgui)