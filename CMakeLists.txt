cmake_minimum_required(VERSION 3.16)
project(the-old-game LANGUAGES CXX)

# CMake 4.x: raylib uses old cmake_minimum_required; allow it (set before FetchContent)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

set(CMAKE_CXX_STANDARD 17)

# raylib via FetchContent – use latest stable (no system install required)
include(FetchContent)
set(RAYLIB_VERSION 5.0)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG ${RAYLIB_VERSION}
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
message(STATUS "Fetching and configuring raylib ${RAYLIB_VERSION} (first run: 1-2 min, git clone)...")
FetchContent_MakeAvailable(raylib)
message(STATUS "raylib ready.")

# raygui (header-only, for sidebar GUI)
FetchContent_Declare(
  raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui.git
  GIT_TAG        master
)
FetchContent_Populate(raygui)
set(RAYGUI_SRC_DIR ${raygui_SOURCE_DIR}/src)
message(STATUS "raygui source: ${RAYGUI_SRC_DIR}")

add_executable(spiel-des-lebens src/main.cpp)
target_include_directories(spiel-des-lebens PRIVATE ${RAYGUI_SRC_DIR})
target_link_libraries(spiel-des-lebens PRIVATE raylib)

# Windows: GUI executable (no console)
if(WIN32)
  set_target_properties(spiel-des-lebens PROPERTIES
    WIN32_EXECUTABLE TRUE
  )
endif()

# macOS: plain executable (no .app bundle for CI)
if(APPLE)
  set_target_properties(spiel-des-lebens PROPERTIES
    MACOSX_BUNDLE FALSE
  )
endif()
