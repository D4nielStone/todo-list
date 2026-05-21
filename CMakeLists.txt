cmake_minimum_required(VERSION 3.16)
project(TLB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BGUI_BUILD_EXAMPLES OFF)
set(BGUI_TESTS OFF)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/nlohman/include
)
add_subdirectory(external/cpp-bgui)

set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)

add_subdirectory(external/cpr)
add_subdirectory(external/nlohman)
add_subdirectory(desktop)
add_subdirectory(backend)