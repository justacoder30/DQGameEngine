cmake_minimum_required(VERSION 3.22)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "4.0")
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

project(DQGameEngine
    VERSION 0.1.0
    DESCRIPTION "DQGameEngine - 2D C++ game engine"
    LANGUAGES C CXX
)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------

# We build external libraries from source so a fresh clone does not need
# pre-installed .lib/.dll files or Visual Studio include/library settings.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build external libraries as static libraries" FORCE)

# SDL3: keep the main SDL library shared so the runtime only needs SDL3.dll.
set(SDL_SHARED ON CACHE BOOL "Build SDL3 shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL3 static library" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build SDL3 test library" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Build SDL3 tests" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Build SDL3 examples" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "Install SDL3" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.12
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3)

# SDL3_mixer: static + vendored codecs, no separate codec DLLs required.
set(SDLMIXER_VENDORED ON CACHE BOOL "Use vendored SDL_mixer dependencies" FORCE)
set(SDLMIXER_DEPS_SHARED OFF CACHE BOOL "Link audio codecs statically" FORCE)
# Override values cached by earlier configurations too.
foreach(codec FLAC_LIBFLAC GME MOD_XMP MP3_MPG123 MIDI_FLUIDSYNTH OPUS VORBIS_VORBISFILE VORBIS_TREMOR WAVPACK)
    set(SDLMIXER_${codec}_SHARED OFF CACHE BOOL "Link ${codec} statically" FORCE)
endforeach()
set(SDLMIXER_TESTS OFF CACHE BOOL "Build SDL3_mixer tests" FORCE)
set(SDLMIXER_EXAMPLES OFF CACHE BOOL "Build SDL3_mixer examples" FORCE)
set(SDLMIXER_INSTALL OFF CACHE BOOL "Install SDL3_mixer" FORCE)

FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-3.2.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3_mixer)

# GLM is header-only for our use.
set(GLM_BUILD_TESTS OFF CACHE BOOL "Build GLM tests" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "Install GLM" FORCE)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

# tmxlite contains pugixml/miniz in its source tree, so no system package is needed.
set(TMXLITE_STATIC_LIB ON CACHE BOOL "Build tmxlite as a static library" FORCE)
set(USE_EXTLIBS OFF CACHE BOOL "Do not use external tmxlite dependencies" FORCE)
set(USE_ZSTD OFF CACHE BOOL "Do not require Zstd for tmxlite" FORCE)

FetchContent_Declare(
    tmxlite
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG v1.4.4
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR tmxlite
)

FetchContent_MakeAvailable(tmxlite)
# tmxlite has no install option; keep its SDK install rules out of our demo.
set_property(DIRECTORY "${tmxlite_SOURCE_DIR}/tmxlite" PROPERTY EXCLUDE_FROM_ALL TRUE)

# Create our own stable target name for the engine.
if(TARGET tmxlite)
    target_compile_definitions(tmxlite PUBLIC TMXLITE_STATIC)
    add_library(DQGame::tmxlite ALIAS tmxlite)
else()
    message(FATAL_ERROR "tmxlite target was not created")
endif()

# Box2D: library only. Samples/tests/docs are not needed by the engine.
set(BOX2D_SAMPLES OFF CACHE BOOL "Build Box2D samples" FORCE)
set(BOX2D_UNIT_TESTS OFF CACHE BOOL "Build Box2D unit tests" FORCE)
set(BOX2D_BENCHMARKS OFF CACHE BOOL "Build Box2D benchmarks" FORCE)
set(BOX2D_DOCS OFF CACHE BOOL "Build Box2D docs" FORCE)
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.1.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(box2d)
set_property(DIRECTORY "${box2d_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL TRUE)

# -----------------------------------------------------------------------------
# Platform libraries
# -----------------------------------------------------------------------------

find_package(OpenGL REQUIRED)

# -----------------------------------------------------------------------------
# DQGameEngine library
# -----------------------------------------------------------------------------

file(GLOB_RECURSE DQGAMEENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/DQGameEngine/DQEngine/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DQGameEngine/DQEngine/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/DQGameEngine/DQEngine/*.h"
)

add_library(DQGameEngine STATIC ${DQGAMEENGINE_SOURCES}
    external/glad/glad.c
    external/stb_image.cpp
)

add_library(DQGame::DQGameEngine ALIAS DQGameEngine)

set_target_properties(DQGameEngine PROPERTIES
    OUTPUT_NAME "DQGameEngine"
)

target_precompile_headers(DQGameEngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/DQGameEngine/DQEngine/pch.h"
)

set_source_files_properties(
    external/glad/glad.c
    external/stb_image.cpp
    PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

# Public because DQEngine.h and public engine headers expose SDL/GLM/tmxlite types.
target_include_directories(DQGameEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/DQGameEngine>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/DQGameEngine/DQEngine>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external>

        $<INSTALL_INTERFACE:include>
)

target_compile_features(DQGameEngine PUBLIC cxx_std_20)

target_compile_definitions(DQGameEngine
    PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

target_link_libraries(DQGameEngine
    PUBLIC
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
        glm::glm
        DQGame::tmxlite
        box2d::box2d
        OpenGL::GL
)

if(MSVC)
    target_compile_options(DQGameEngine PRIVATE /W3 /permissive-)
endif()

# -----------------------------------------------------------------------------
# Sandbox
# -----------------------------------------------------------------------------

option(
    DQGAMEENGINE_BUILD_SANDBOX
    "Build the engine Sandbox"
    ${PROJECT_IS_TOP_LEVEL}
)

if(
    DQGAMEENGINE_BUILD_SANDBOX
    AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Sandbox/CMakeLists.txt"
)
    add_subdirectory(Sandbox)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Sandbox)
endif()

option(DQGAMEENGINE_INSTALL_SDK "Install the Windows Visual Studio engine SDK" OFF)
if(DQGAMEENGINE_INSTALL_SDK)
    include(cmake/InstallSDK.cmake)
endif()
