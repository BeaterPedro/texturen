cmake_minimum_required(VERSION 3.20)
project(ZombieEngine VERSION 0.1.0 LANGUAGES CXX C)

# ─────────────────────────────────────────────────────────────
#  C++ Standard
# ─────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────────────────────────
#  Ausgabeverzeichnisse
# ─────────────────────────────────────────────────────────────
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ─────────────────────────────────────────────────────────────
#  Abhängigkeiten via FetchContent (automatischer Download)
# ─────────────────────────────────────────────────────────────
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE) # Fortschritt sichtbar machen

# ── GLFW 3.4 ── Fenster & Input ──────────────────────────────
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# ── GLM 1.0.1 ── Mathematik (Header-Only) ───────────────────
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(glm)

# ── GLAD 2 ── OpenGL 4.1 Core Loader ────────────────────────
# Moderner CMake-nativer Ansatz, kein Python nötig
FetchContent_Declare(
    glad2
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.7
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad2)
glad_add_library(glad_gl REPRODUCIBLE LOADER LANGUAGE C API gl:core=4.1)

# OpenGL (Systembibliotek)
find_package(OpenGL REQUIRED)

# ─────────────────────────────────────────────────────────────
#  Quelldateien sammeln
# ─────────────────────────────────────────────────────────────
file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp"
)

add_executable(ZombieEngine ${ENGINE_SOURCES})

# GLFW_INCLUDE_NONE verhindert Konflikte zwischen GLFW und GLAD
target_compile_definitions(ZombieEngine PRIVATE GLFW_INCLUDE_NONE)

target_include_directories(ZombieEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ZombieEngine PRIVATE
    glfw
    glm::glm
    glad_gl
    OpenGL::GL
)

# Warnungen aktivieren (Compiler-abhängig)
if(MSVC)
    target_compile_options(ZombieEngine PRIVATE /W4)
else()
    target_compile_options(ZombieEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ─────────────────────────────────────────────────────────────
#  Shader nach Build-Verzeichnis kopieren
# ─────────────────────────────────────────────────────────────
add_custom_command(TARGET ZombieEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:ZombieEngine>/shaders
    COMMENT "Kopiere Shader ins Build-Verzeichnis..."
)

# ─────────────────────────────────────────────────────────────
#  Assets nach Build-Verzeichnis kopieren (später für Modelle)
# ─────────────────────────────────────────────────────────────
add_custom_command(TARGET ZombieEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:ZombieEngine>/assets
    COMMENT "Kopiere Assets ins Build-Verzeichnis..."
)

message(STATUS "ZombieEngine v${PROJECT_VERSION} konfiguriert")
message(STATUS "  Build-Typ : ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++       : C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Output    : ${CMAKE_BINARY_DIR}/bin")
