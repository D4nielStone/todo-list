cmake_minimum_required(VERSION 3.28)
project(TodoList)

add_subdirectory(external/cpp-bgui)
add_executable(app main.cpp)
target_link_libraries(app PRIVATE bgui)