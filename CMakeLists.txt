cmake_minimum_required(VERSION 3.28)
project(TodoList)

set(BGUI_BUILD_EXAMPLES OFF)
set(BGUI_TESTS OFF)
add_executable(app main.cpp)

add_subdirectory(external/cpp-bgui)
target_link_libraries(app PRIVATE bgui sqlite3)