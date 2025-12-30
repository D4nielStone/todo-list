cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(TodoList)

set(BGUI_BUILD_EXAMPLES OFF)
set(BGUI_TESTS OFF)
add_executable(app main.cpp)

add_subdirectory(external/cpp-bgui)

find_package(SQLite3 REQUIRED)

target_link_libraries(app PRIVATE SQLite::SQLite3 bgui)