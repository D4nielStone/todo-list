cmake_minimum_required(VERSION 3.16)
project(TLB)

if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
elseif(EXISTS "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
elseif(EXISTS "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" CACHE STRING "")
endif()

# Set the target triplet for MSVC (Visual Studio)
if(WIN32 AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET x64-windows CACHE STRING "Target triplet for vcpkg")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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