# Getting started with DQGameEngine

DQGameEngine is a C++20 engine for 2D games. You write C++ components, add them
to a `dqengine::GameApp`, and let the application update and draw them.
The supported SDK packaging workflow is Windows x64 with MSVC and OpenGL 3.3.

Read this page first, then the [user guide](user-guide.md) and
[API reference](api-reference.md). All examples use the current `dqengine`
namespace and smart-pointer API.

## Run the existing game

Install Visual Studio with the **Desktop development with C++** workload, a
Windows SDK, CMake 3.22 or newer, and Git. The first configure needs internet
access to fetch dependencies. From the repository root:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release --parallel
cd build/Sandbox/Release
.\Sandbox.exe
```

The CMake build copies the demo resources and SDL3.dll beside Sandbox.exe.
Run with that directory as the working directory. You can also open the generated
solution in `build`, choose x64 and Release, and run the Sandbox startup project.
Depending on CMake and Visual Studio versions, the solution extension is `.sln`
or `.slnx`. Use the generated solution for this workflow.

`cmake -S ... -B ...` configures the project; `cmake --build ...` compiles it.
To install the compiled demo, return to the repository root and run:

```powershell
cmake --install build --config Release --prefix ./install
cd install/bin
.\Sandbox.exe
```

Use the same configuration for building and installing.

## Use the engine in your own Visual Studio project

If you received an SDK folder, skip the following producer commands. If you
have the source repository, build a package without Sandbox from the root:

```powershell
cmake -S . -B build-sdk -A x64 -DDQGAMEENGINE_BUILD_SANDBOX=OFF -DDQGAMEENGINE_INSTALL_SDK=ON
cmake --build build-sdk --config Release --target DQGameEngine --parallel
cmake --install build-sdk --config Release --component EngineSDK --prefix ./install/DQGameEngineSDK
```

To supply Debug as well:

```powershell
cmake --build build-sdk --config Debug --target DQGameEngine --parallel
cmake --install build-sdk --config Debug --component EngineSDK --prefix ./install/DQGameEngineSDK
```

Distribute the whole SDK folder, including the headers, libraries, SDL3 runtime,
property sheets, documentation, and licenses. The engine library alone is not
a complete package.

1. Create a C++ Console Application in Visual Studio and select x64.
2. Use the compiler toolset recorded in the SDK's top-level README. Rebuild the
   SDK if you need a different toolset.
3. In **View > Other Windows > Property Manager**, right-click **Release | x64**,
   choose **Add Existing Property Sheet**, and select the SDK's `DQGameEngine.props`.
   Repeat for **Debug | x64** if the SDK contains Debug libraries.
4. Disable precompiled headers for the example source if the project requires
   a generated `pch.h`. Replace the generated main source with
   [the quickstart main.cpp](examples/quickstart/main.cpp).
5. Build and run. Move the blue square with A/D or the arrow keys. Close the
   window to exit.

The property sheet sets C++20, include paths, dependency libraries, `TMXLITE_STATIC`,
and `/MD` or `/MDd`. It also copies SDL3.dll into your output directory.
Avoid conflicting project overrides and do not mix Debug and Release libraries.

Paths inside the property sheet are resolved relative to that sheet's directory.
If you move the SDK, update your project's import path to `DQGameEngine.props`.
You do not need to edit each include and library path inside the SDK.

## Your first application

The complete example is deliberately asset-free: the default `SpriteComponent`
uses the engine's white texture, tinted blue. Set its size explicitly.

```cpp
#include <DQEngine/DQEngine.h>

class MyGame : public dqengine::GameApp {
public:
    MyGame() : GameApp(800, 450, "My game") {}

protected:
    void OnLoad() override {
        auto* sprite = Add(dqengine::Unique<dqengine::SpriteComponent>());
        sprite->position = dqengine::Vector(100.f, 100.f);
        sprite->size = dqengine::Vector(48.f, 48.f);
        auto* camera = Add(dqengine::Unique<dqengine::CameraComponent>(800.f, 450.f));
        camera->SetOrigin(dqengine::Vector(0.f, 0.f));
    }
};

int main() {
    MyGame game;
    game.Run();
    return 0;
}
```

`GameApp` creates the window, OpenGL context, renderer, and default camera.
`Run()` loads the component tree and starts the game loop. Create sprites,
animations, textures, and tile maps after constructing the application; `OnLoad`
is a convenient place. Do not create GPU resources as global variables.

To show an image, pass its path to the sprite constructor:

```cpp
auto* sprite = Add(dqengine::Unique<dqengine::SpriteComponent>("resource/player.png"));
sprite->size = dqengine::Vector(64.f, 64.f);
```

This snippet belongs inside a component's `OnLoad`. Image paths are relative to
the process working directory. Copy your `resource` directory beside your
executable and set **Debugging > Working Directory** to `$(OutDir)` when using
that layout. The SDK copies SDL3.dll, but your own assets are your responsibility.
The built-in renderer currently embeds its default shader source, so this
example does not require external shader files.

The example attaches a camera with origin `(0,0)` so world `(0,0)` is the
top-left of the window. Cameras otherwise default to a centered origin.

## Use the source as a CMake dependency

For a separate source-based game, a minimal CMakeLists.txt can use:

```cmake
cmake_minimum_required(VERSION 3.22)
project(MyGame LANGUAGES C CXX)

set(DQGAMEENGINE_BUILD_SANDBOX OFF CACHE BOOL "" FORCE)
set(DQGAMEENGINE_INSTALL_SDK OFF CACHE BOOL "" FORCE)
add_subdirectory(path/to/DQGameEngine engine)

add_executable(MyGame main.cpp)
target_compile_features(MyGame PRIVATE cxx_std_20)
target_link_libraries(MyGame PRIVATE DQGame::DQGameEngine)
if(WIN32)
    add_custom_command(TARGET MyGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_FILE_DIR:MyGame>")
endif()
set_property(TARGET MyGame PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
    "$<TARGET_FILE_DIR:MyGame>")
```

Replace `path/to/DQGameEngine` with the source checkout location. This is a
source-build workflow; the packaged SDK is consumed through its Visual Studio
property sheet. There is currently no installed CMake `find_package` config.

## Troubleshooting

- **Cannot find an engine header:** import the SDK property sheet for the active
  configuration, or link the CMake `DQGame::DQGameEngine` target.
- **Unknown `GameApp` or `Vector`:** use `dqengine::GameApp` and `dqengine::Vector`.
  All engine names are inside `dqengine`.
- **Unresolved symbols or runtime mismatch:** check x64, compiler toolset,
  Debug/Release, and `/MD` versus `/MDd`. Replace SDK headers and libraries together.
- **SDL3.dll missing:** verify the property sheet is imported and the DLL was
  copied beside the executable.
- **Image not found or invisible sprite:** check the working directory, the
  console's asset error, explicit nonzero size, visibility, and camera position.
- **Crash when creating a sprite:** construct `GameApp` before GPU resources.
- **Install cannot find a library:** build the same configuration before install.

Next: [components and the rest of the engine](user-guide.md).
