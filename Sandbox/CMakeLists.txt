# =============================================================================
# Sandbox
# =============================================================================

file(GLOB_RECURSE SANDBOX_SOURCES
    CONFIGURE_DEPENDS

    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(Sandbox ${SANDBOX_SOURCES})

# -----------------------------------------------------------------------------
# Engine
# -----------------------------------------------------------------------------

target_link_libraries(Sandbox PRIVATE DQGame::DQGameEngine)

target_compile_features(Sandbox PRIVATE cxx_std_20)

# -----------------------------------------------------------------------------
# Include directories
# -----------------------------------------------------------------------------

target_include_directories(Sandbox PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# -----------------------------------------------------------------------------
# MSVC
# -----------------------------------------------------------------------------

if(MSVC)

    target_compile_options(Sandbox
        PRIVATE
            /W3
            /permissive-
            /utf-8
    )

endif()

# Install a runnable demo, including its assets and the shared SDL runtime.
install(TARGETS Sandbox RUNTIME DESTINATION bin COMPONENT Sandbox)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resource"
    DESTINATION bin COMPONENT Sandbox)
if(WIN32)
    install(FILES "$<TARGET_FILE:SDL3::SDL3-shared>"
        DESTINATION bin COMPONENT Sandbox)
endif()

# -----------------------------------------------------------------------------
# Visual Studio
# -----------------------------------------------------------------------------

# Makes the source tree appear nicely in Solution Explorer.
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    PREFIX "Sandbox"
    FILES ${SANDBOX_SOURCES}
)

# Make Visual Studio launch Sandbox with the executable directory
# as its working directory.
set_property(
    TARGET Sandbox
    PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
        "$<TARGET_FILE_DIR:Sandbox>"
)

# =============================================================================
# Runtime files
# =============================================================================

# -----------------------------------------------------------------------------
# Sandbox resources
# -----------------------------------------------------------------------------
#
# Your game code uses paths such as:
#
#   resource/Map/map2.tmx
#   resource/img/Player/...
#   resource/img/Enemy/...
#
# Therefore copy Sandbox/resource beside Sandbox.exe.
# -----------------------------------------------------------------------------

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resource")

    add_custom_command(
        TARGET Sandbox
        POST_BUILD

        COMMAND
            ${CMAKE_COMMAND} -E copy_directory

            "${CMAKE_CURRENT_SOURCE_DIR}/resource"

            "$<TARGET_FILE_DIR:Sandbox>/resource"

        VERBATIM
    )

endif()

# -----------------------------------------------------------------------------
# SDL3.dll
# -----------------------------------------------------------------------------

if(WIN32)

    add_custom_command(
        TARGET Sandbox
        POST_BUILD

        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different

            "$<TARGET_FILE:SDL3::SDL3-shared>"

            "$<TARGET_FILE_DIR:Sandbox>"

        VERBATIM
    )

endif()
