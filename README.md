Todo List (using bgui)
===

<a href="https://github.com/D4nielStone/todo-list/releases/">![GitHub Tag](https://img.shields.io/github/v/tag/D4nielStone/todo-list?style=flat-square&label=Latest)</a>

Is a ToDo List app made in cpp using my GUI framework, <a href="https://www.github.com/D4nielStone/cpp-bgui" target="_blank">Bubble GUI</a>.<br>
This Software uses sqlite3 to store database. If you want to compile, please make sure you have installed sqlite-devel lib in your system.

<center><img width="600" height="475" alt="1267409194d39d3dad74763a5faaa89e6997e1fb" src="https://github.com/user-attachments/assets/ce4a9f5a-9fc7-4f85-8344-63adc41117bb" /></center>

# Building todo-list — Ubuntu and Windows

This file describes how to build the repository on Ubuntu (Linux) and Windows based on the repository's CI workflow (.github/workflows/cmake-multi-platform.yml).

General notes
- The project uses CMake. The workflow sets `CMAKE_C_COMPILER`, `CMAKE_CXX_COMPILER`, and (on Windows) a vcpkg toolchain file.
- The repository uses git submodules — make sure to initialize them before configuring CMake.

---

## Common first steps (both systems)

1. Clone the repo and initialize submodules:
```bash
git clone https://github.com/D4nielStone/todo-list.git
cd todo-list
git submodule update --init --recursive
```

2. Create a build directory (recommended):
```bash
mkdir -p build
```

---

## Ubuntu (bash)

1. Install OS packages used in the workflow:
```bash
sudo apt-get update
sudo apt-get install -y \
  libsqlite3-dev \
  libglfw3-dev \
  libfreetype6-dev \
  xorg-dev \
  libx11-dev \
  libxrandr-dev \
  libxinerama-dev \
  libxcursor-dev \
  libxi-dev
```

2. Choose compiler:
- GCC: `gcc` / `g++`
- Clang: `clang` / `clang++`

3. Configure CMake (example using GCC and Release build):
```bash
cmake -B build \
  -DCMAKE_C_COMPILER=gcc \
  -DCMAKE_CXX_COMPILER=g++ \
  -DCMAKE_BUILD_TYPE=Release \
  -DEXTERNALS_DIR="${PWD}/externals" \
  -DCPP_BGUI_DIR="${PWD}/externals/cpp-bgui" \
  -S "${PWD}"
```

(Replace `gcc`/`g++` with `clang`/`clang++` if you want Clang.)

4. Build:
```bash
cmake --build build --config Release
```

5. (Optional) See submodule status if you have issues:
```bash
git submodule status --recursive
```

---

## Windows (PowerShell / cmd)

The CI uses vcpkg to install and manage dependencies on Windows.

1. Clone vcpkg and bootstrap (PowerShell example):
```powershell
git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat -disableMetrics
```

2. Install required packages via vcpkg (x64 example from workflow):
```powershell
.\vcpkg\vcpkg install glfw3:x64-windows
.\vcpkg\vcpkg install freetype:x64-windows
.\vcpkg\vcpkg install sqlite3:x64-windows
.\vcpkg\vcpkg integrate install
```

3. Configure CMake (PowerShell example). The workflow uses the vcpkg toolchain file and MSVC (`cl`) on Windows:
```powershell
cmake -B "${PWD}\build" `
  -DCMAKE_CXX_COMPILER=cl `
  -DCMAKE_C_COMPILER=cl `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE="${PWD}\vcpkg\scripts\buildsystems\vcpkg.cmake" `
  -DEXTERNALS_DIR="${PWD}\externals" `
  -DCPP_BGUI_DIR="${PWD}\externals\cpp-bgui" `
  -S "${PWD}"
```

If using the Visual Studio generator explicitly (optional), you can add `-G "Visual Studio 17 2022" -A x64` (match your installed VS version).

4. Build (PowerShell or cmd):
```powershell
cmake --build .\build --config Release
```

---

## Tips and troubleshooting

- If CMake can't find libraries on Linux, verify the dev packages are installed (e.g., `libsqlite3-dev`, `libglfw3-dev`, etc.).
- On Windows, ensure you run PowerShell from a Developer Command Prompt (so `cl` is available) or use the Visual Studio generator so MSVC tools are on PATH.
- If vcpkg installs packages for a specific triplet (e.g. `x64-windows`), make sure your CMake generator and build architecture match that triplet (x64 vs x86).
- If you change compilers manually, set `-DCMAKE_C_COMPILER` and `-DCMAKE_CXX_COMPILER` as shown in the examples.
