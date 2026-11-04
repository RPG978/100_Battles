# 100 Battles - Build & Dependency Notes

## Purpose

This document describes the build requirements, dependencies, and setup process for developers who want to build 100_Battles from source.

The goal is to make the project buildable without relying on Visual Studio project settings or machine-specific configurations.

---

# Project Overview

100_Battles is a C++20 project using:

- SDL2
- OpenGL
- ImGui
- SQLite (embedded)
- Catch2 (tests)

Build system:

- CMake 3.16+

Primary executable:

- game_main

---

# Repository Structure

```text
100_Battles/
├── data/
│   ├── game.db
│   ├── enemies.csv
│   └── ...
├── external/
│   ├── imgui/
│   └── SDL2/
├── include/
├── src/
├── tests/
├── CMakeLists.txt
├── README.md
└── 100_battles_dependencies.md
```

---

# Dependencies

## SDL2

SDL2 is provided through vcpkg.

Install:

```powershell
vcpkg install sdl2:x64-windows
```

Verification:

```powershell
vcpkg list
```

Should contain:

```text
sdl2:x64-windows
```

Notes:

- The project uses:

```cmake
find_package(SDL2 CONFIG REQUIRED)
```

- SDL2 is discovered through the vcpkg toolchain.
- Manual Visual Studio SDL include paths are no longer required for CMake builds.

---

## OpenGL

Required for ImGui rendering.

Resolved through:

```cmake
find_package(OpenGL REQUIRED)
```

No additional setup currently required on Windows.

---

## ImGui

ImGui is vendored directly into the repository.

Location:

```text
external/imgui/
```

Built from source by CMake.

No separate installation required.

---

## SQLite

SQLite source is embedded directly into the project.

Location:

```text
src/sqlite3.c
```

No installation required.

---

## Catch2

Used for unit tests.

Downloaded automatically through:

```cmake
FetchContent
```

No manual installation required.

---

# Build Requirements

## Compiler

Confirmed working:

- MSVC 2022

Language Standard:

```text
C++20
```

---

## Required Software

- Visual Studio 2022
- CMake 3.16+
- vcpkg

---

# One-Time Setup

Install SDL2:

```powershell
cd C:\Users\Chris\vcpkg

.\vcpkg install sdl2:x64-windows

.\vcpkg integrate install
```

---

# Build

From repository root:

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=C:/Users/Chris/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Build:

```powershell
cmake --build build
```

---

# Run

```powershell
.\build\Debug\game_main.exe
```

---

# Tests

Tests are built automatically.

Executable:

```text
build/Debug/tests.exe
```

Run:

```powershell
.\build\Debug\tests.exe
```

---

# Runtime Assets

The following files must be present:

```text
data/
├── game.db
├── enemies.csv
├── equipment.csv
└── ...
```

The game depends on these files at runtime.

---

# Known Notes

## SDL Main Handling

SDL2 and Windows entry-point behavior required:

```cpp
#define SDL_MAIN_HANDLED
```

before SDL initialization.

Without this define, command-line CMake builds may encounter linker issues involving:

```text
main
SDL_main
```

---

# Future Work

Planned improvements:

- Linux build verification
- Docker support
- CI documentation
- Release packaging instructions
- Automated dependency setup