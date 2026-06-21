cmake_minimum_required(VERSION 3.16)
project(TLB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32 AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic CACHE STRING "")
endif()

set(BGUI_BUILD_EXAMPLES OFF)
set(BGUI_TESTS OFF)
set(BGUI_USE_VUKAN OFF)
set(BGUI_USE_FREETYPE ON)

include_directories(
    ${CMAKE_SOURCE_DIR}
)


add_subdirectory(external/cpp-bgui)
add_subdirectory(external/cpr)
add_subdirectory(external/nlohman)
find_package(SQLite3 REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(desktop)