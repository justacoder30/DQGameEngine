# A relocatable SDK for native MSBuild projects. Static archives do not absorb
# their linked libraries, so ship the dependency archives as well as the engine.
if(NOT MSVC OR NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "DQGAMEENGINE_INSTALL_SDK currently requires Windows x64 and MSVC")
endif()

set(sdk_targets DQGameEngine SDL3::SDL3-shared SDL3_mixer::SDL3_mixer
    glm::glm tmxlite box2d::box2d)
# These are the vendored codec targets from the pinned SDL_mixer version.
# Disabled backends have no target and do not need an archive in the SDK.
foreach(codec opusfile opus vorbisfile vorbis ogg FLAC gme_static
              xmp_static libmpg123 wavpack vorbisidec)
    if(TARGET ${codec})
        list(APPEND sdk_targets ${codec})
    endif()
endforeach()

set(sdk_libraries "")
foreach(sdk_target IN LISTS sdk_targets)
    get_target_property(sdk_type ${sdk_target} TYPE)
    if(sdk_type STREQUAL "INTERFACE_LIBRARY")
        continue()
    endif()
    install(FILES "$<TARGET_LINKER_FILE:${sdk_target}>"
        DESTINATION "lib/$<CONFIG>" COMPONENT EngineSDK)
    string(APPEND sdk_libraries "$<TARGET_LINKER_FILE_NAME:${sdk_target}>;")
endforeach()
string(APPEND sdk_libraries "opengl32.lib;shlwapi.lib")

install(FILES "$<TARGET_FILE:SDL3::SDL3-shared>"
    DESTINATION "bin/$<CONFIG>" COMPONENT EngineSDK)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/DQGameEngine/DQEngine"
    DESTINATION include COMPONENT EngineSDK FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/external/glad" "${PROJECT_SOURCE_DIR}/external/KHR"
    DESTINATION include COMPONENT EngineSDK FILES_MATCHING PATTERN "*.h")
install(FILES "${PROJECT_SOURCE_DIR}/external/stb_image.h"
    DESTINATION include COMPONENT EngineSDK)
install(DIRECTORY "${sdl3_SOURCE_DIR}/include/SDL3" "${sdl3_mixer_SOURCE_DIR}/include/SDL3_mixer"
    "${tmxlite_SOURCE_DIR}/tmxlite/include/tmxlite" "${box2d_SOURCE_DIR}/include/box2d"
    DESTINATION include COMPONENT EngineSDK FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")
install(DIRECTORY "${glm_SOURCE_DIR}/glm" DESTINATION include COMPONENT EngineSDK
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")

# Keep Debug and Release side by side, including configuration-specific names.
configure_file("${CMAKE_CURRENT_LIST_DIR}/DQGameEngine.props.in"
    "${PROJECT_BINARY_DIR}/sdk/DQGameEngine.props.in" @ONLY)
file(GENERATE OUTPUT "${PROJECT_BINARY_DIR}/sdk/$<CONFIG>/DQGameEngine.$<CONFIG>.props"
    INPUT "${PROJECT_BINARY_DIR}/sdk/DQGameEngine.props.in")
install(FILES "${PROJECT_BINARY_DIR}/sdk/$<CONFIG>/DQGameEngine.$<CONFIG>.props"
    DESTINATION . COMPONENT EngineSDK)
install(FILES "${CMAKE_CURRENT_LIST_DIR}/DQGameEngine.props"
    DESTINATION . COMPONENT EngineSDK)
configure_file("${CMAKE_CURRENT_LIST_DIR}/SDK-README.md"
    "${PROJECT_BINARY_DIR}/sdk/README.md" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/sdk/README.md"
    DESTINATION . RENAME README.md COMPONENT EngineSDK)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/docs/"
    DESTINATION docs COMPONENT EngineSDK)
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE"
    DESTINATION licenses/DQGameEngine COMPONENT EngineSDK)
# Preserve upstream notices, including those of vendored audio codecs.
foreach(dependency sdl3 sdl3_mixer glm tmxlite box2d)
    set(dependency_source "${${dependency}_SOURCE_DIR}")
    file(GLOB_RECURSE notices RELATIVE "${dependency_source}"
        "${dependency_source}/LICENSE*" "${dependency_source}/COPYING*"
        "${dependency_source}/COPYRIGHT*")
    foreach(notice IN LISTS notices)
        get_filename_component(notice_dir "${notice}" DIRECTORY)
        install(FILES "${dependency_source}/${notice}"
            DESTINATION "licenses/${dependency}/${notice_dir}" COMPONENT EngineSDK)
    endforeach()
endforeach()
