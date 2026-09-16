# DQGameEngine 🎮

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3%2B-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20x64-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

**DQGameEngine** is a custom 2D C++ game engine designed for educational and performance-focused game development. The repository includes both the core engine architecture and a **Sandbox** project demonstrating the engine's capabilities.

## Sandbox Demo Image
![Sandbox Demo](.github/assets/demo_img.png)

## 🎮 Sandbox Controls

You can test the engine's movement, FSM state machine, and collision system using the pre-configured controls in the Sandbox project:

| Key Action | Input | Description |
| :--- | :--- | :--- |
| **Move Left / Right** | `A` / `D` | Move character horizontally |
| **Jump** | `Space` | Jump vertically |
| **Attack** | `J` | Perform ground attack |
| **Air Attack** | `J` *(while airborne)* | Execute aerial attack |
| **Roll / Dodge** | `Shift` *(while moving)* | Perform a dodge roll |

## DarkMage boss

DarkMage keeps its distance and alternates telegraphed head casts with skeleton
summons. Attack2 uses `Head.png`: heads track briefly, then fly straight, can be
destroyed by a player attack, and stop at terrain. Rolling avoids their damage.
Below half health, DarkMage moves faster, recovers sooner, and fires two heads.
Heal has a green tint and can be interrupted by hitting the boss before
the healing frame. There are two healing attempts and four summons per boss.
Leaving the encounter or killing the boss clears its projectiles.

Health, movement range, cooldowns, and encounter limits are in `Sandbox/DarkMage.h`;
cast/release frames are in `Sandbox/DarkMageState/DarkMageAttackState.cpp`.

`Sandbox/Head.h` and `Head.cpp` implement the flying projectile as a
`SpriteComponent`. DarkMage spawns it with `GetParent()->QueueAdd(Unique<Head>(...))`.
`QueueAdd` takes ownership immediately, attaches after the parent's traversal,
and starts updating the new child on the next frame. `Add` also queues
automatically when the parent is traversing children. Heads call
`RemoveFromParent()` on impact or expiry. A weak volley reference cancels them
when the boss disengages or dies without keeping a dangling boss pointer.

## 🌟 Features

### Namespace and smart pointers

All engine types and functions are in `dqengine`. Include `<DQEngine/DQEngine.h>`:

```cpp
dqengine::Component scene;
auto sprite = dqengine::Unique<dqengine::SpriteComponent>("resource/player.png");
auto* playerSprite = scene.Add(std::move(sprite));
playerSprite->position = dqengine::Vector(100.f, 100.f);
```

`Unique<T>(args...)` and `Shared<T>(args...)` are factories. Use `UniquePtr<T>`,
`SharedPtr<T>`, and `WeakPtr<T>` for stored pointer types. Components own children
with unique pointers; `Add` and `QueueAdd` transfer ownership. Their returned
pointers, `GetParent()`, and `GetComponent<T>()` are borrowed references: do not
delete them or keep using them after removal. `GetChildren()` exposes a const
vector of unique pointers; use `child.get()` when a borrowed pointer is needed.
Legacy `Add(Component*)` still adopts ownership for existing games.

Textures are shared between the cache, sprites, and animations. Clearing the
cache no longer destroys textures still in use. Release game resources before
the `GameApp` (and its OpenGL context) is destroyed. State machines own states
created through `CreateState<T>(args...)`; `ChangeState` borrows its argument.

This namespace/ownership change requires rebuilding the SDK and consumer projects
together. Existing code must qualify engine names or use `using namespace
dqengine;` in its own source files. Sandbox uses `Engine.h` for this convenience.

- **2D Rendering Engine:** Modern OpenGL pipeline with Glad loader, supporting 2D sprite batching, custom shader management, and texture atlases.
- **2D Camera System:** Dynamic 2D camera supporting target tracking, smooth follow (Lerp), zoom controls, and viewport boundary clamping.
- **Animation System:** Frame-based sprite-sheet animation manager with configurable playback rates, integrated with a Finite State Machine (FSM) for character state transitions.
- **Collision Detection & Spatial Partitioning:** Axis-Aligned Bounding Box (AABB) collision detection and resolution algorithms, optimized with a **Spatial Grid** for fast broad-phase collision filtering and dynamic entity partitioning.
- **Windowing & Input:** Powered by **SDL3** for cross-platform window handling, graphics context creation, and real-time event processing.
- **Tilemap Support:** Native `.tmx` Tiled map parsing and multi-layer rendering integrated with **tmxlite**.
- **Math Library:** Hardware-accelerated vector and matrix calculations powered by **GLM**.

## ⚙️ Installation & Setup

### Prerequisites
- **Operating System:** Windows 10 / 11 (64-bit)
- **IDE:** Visual Studio 2022 with **Desktop development with C++** workload installed.

### Build and run Sandbox from source  with CMake

Install CMake 3.22 or newer and Git. The first configure downloads and configures
the dependencies, so it requires internet access and can take several minutes.
Run these commands from the repository root:

```powershell
cmake -S . -B build
cmake --build build --config Release --parallel
cmake --install build --config Release --prefix "./install"
```

Configure creates the Visual Studio solution; it does **not** compile the code.
Installation copies already-built files, so build the same configuration before
installing it. The local install prefix avoids writing into Program Files.
The install contains the Sandbox demo, SDL3 runtime, and game resources;
it is not a standalone engine SDK.

To run the installed demo:

```powershell
cd install/bin
.\Sandbox.exe
```

Alternatively, open `build/DQGameEngine.sln` (or `.slnx` with newer CMake), select Debug or Release and x64,
and press F5. Sandbox is the default startup project, and its runtime files are
copied beside the executable. Installation is not required to run from Visual
Studio. Use the solution inside `build` for this CMake workflow.

For a Debug command-line build, use `--config Debug`. Always run Sandbox with
its executable directory as the working directory so relative asset paths work.
The optional `windows` preset requires Visual Studio 2026; the commands above
let CMake select your installed Visual Studio version.

### Build and package only the engine (Visual Studio SDK)

Turn off Sandbox and enable the optional SDK installation:

```powershell
cmake -S . -B build-sdk -A x64 -DDQGAMEENGINE_BUILD_SANDBOX=OFF -DDQGAMEENGINE_INSTALL_SDK=ON
cmake --build build-sdk --config Release --target DQGameEngine --parallel
cmake --install build-sdk --config Release --component EngineSDK --prefix ./install/DQGameEngineSDK
```

To include Debug libraries in the same package:

```powershell
cmake --build build-sdk --config Debug --target DQGameEngine --parallel
cmake --install build-sdk --config Debug --component EngineSDK --prefix ./install/DQGameEngineSDK
```

Zip and distribute the **whole** `install/DQGameEngineSDK` folder. It includes
`include`, `lib/Release` (and `lib/Debug` when installed), matching SDL3.dll files
in `bin/<Configuration>`, dependency licenses, and `DQGameEngine.props`.
The engine `.lib` does not contain all its linked dependencies by itself.

In the recipient's Visual Studio x64 C++ project, open **View > Other Windows >
Property Manager**, right-click **Release | x64**, select **Add Existing Property
Sheet**, and choose `DQGameEngine.props`. Repeat for Debug if supplied. The sheet
sets include paths, library paths, dependencies, C++20, `TMXLITE_STATIC`, and
the matching `/MD` or `/MDd` runtime; it also copies SDL3.dll next to the game.

Use `#include <DQEngine/DQEngine.h>`. Build the consumer with the same MSVC
toolset used to produce the SDK, and use matching Debug/Release configurations.
The installed README records the producing compiler/toolset. Supply your own
game assets. SDK installation currently supports Windows x64 with MSVC.

If you only need to compile the engine locally, use
`-DDQGAMEENGINE_BUILD_SANDBOX=OFF` without enabling SDK installation.
