# DQGameEngine Visual Studio SDK

This package contains the engine and dependency headers, static libraries,
SDL3 import library and runtime DLL, and Visual Studio property sheets.
The engine library alone is not sufficient: distribute this whole folder.

Start with [Getting started](docs/getting-started.md), then the
[user guide](docs/user-guide.md) and [API reference](docs/api-reference.md).
An asset-free [quickstart example](docs/examples/quickstart/main.cpp) is included;
add it as the main source of your own Visual Studio project after importing the
property sheet below.

## Use in a Visual Studio C++ project

1. Extract this folder anywhere, including a path containing spaces.
2. Create an x64 C++ Console Application. Use the same MSVC toolset and
   Windows SDK as the SDK producer (generator: @CMAKE_GENERATOR@,
   toolset: @CMAKE_VS_PLATFORM_TOOLSET@, compiler: @CMAKE_CXX_COMPILER_VERSION@,
   Windows SDK: @CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION@).
   To support another toolset, rebuild the SDK with that toolset.
3. Open **View > Other Windows > Property Manager**. Under your project,
   right-click **Release | x64**, choose **Add Existing Property Sheet**, and
   select `DQGameEngine.props`. Do the same for **Debug | x64** if included.
4. Use `#include <DQEngine/DQEngine.h>` in your code. All engine names are in
   `dqengine`, for example `dqengine::GameApp` and `dqengine::Vector`.

Use `dqengine::Unique<T>(args...)` / `dqengine::Shared<T>(args...)` to create
smart pointers and `UniquePtr<T>` / `SharedPtr<T>` / `WeakPtr<T>` to store them.
`scene.Add(dqengine::Unique<MyComponent>())` transfers ownership to the scene
and returns a borrowed `MyComponent*`. Never delete that pointer; call
`RemoveFromParent()` to schedule removal. Use `QueueAdd` when spawning during
traversal. `GetChildren()` returns a const vector of unique pointers.
Textures are shared; release them before destroying the game's OpenGL context.
Rebuild consumers when upgrading from the former global-namespace API, and
replace headers and libraries together.

The property sheet sets both include directories, the required libraries,
`TMXLITE_STATIC`, C++20, and the DLL C runtime (`/MD` in Release, `/MDd` in
Debug). It also copies the matching SDL3.dll beside your executable.
Remove conflicting per-project overrides of these settings. Use a Console
subsystem with a normal `main()` entry point. Supply your own game assets and
set the debugger working directory to the folder containing those assets.

Debug and Release libraries must not be mixed. `lib/<Configuration>` and
`bin/<Configuration>` keep each installed configuration separate. Install
additional configurations into this same SDK folder to make them available.
Consumers do not need the engine source tree, CMake, or downloaded build folders.

The `licenses` folder and notices embedded in headers accompany the dependencies.
