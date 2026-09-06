# DQGameEngine 🎮

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3%2B-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20x64-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

**DQGameEngine** is a custom 2D C++ game engine designed for educational and performance-focused game development. The repository includes both the core engine architecture and a **Sandbox** project demonstrating the engine's capabilities.

---

## 🌟 Features

- **Rendering Engine:** Modern OpenGL pipeline with Glad loader, supporting sprite batching and custom shaders.
- **Windowing & Input:** Powered by **SDL3** for cross-platform hardware handling and event processing.
- **Audio & Media:** Multi-channel sound effects and music management using `SDL3_mixer`, font rendering via `SDL3_ttf`, and texture loading with `SDL3_image`.
- **Tilemap Support:** Native `.tmx` map parsing and rendering integrated with **tmxlite**.
- **Animation System:** Finite State Machine (FSM) architecture for character states and sprite-sheet animations.
- **Math Library:** Hardware-accelerated vector and matrix calculations powered by **GLM**.

---

## 📁 Repository Structure

```text
DQGameEngine/
├── DQGameEngine/           # Core Engine Project (Static Library)
│   ├── src/                # Engine source code & core systems
│   ├── externalLib/        # Pre-configured third-party headers & libraries
│   └── assets/             # Core engine assets & default shaders
├── Sandbox/                # Demo Application Project (.exe)
│   ├── PlayerState/        # FSM implementation for player entity
│   ├── EnemyState/         # FSM implementation for AI entities
│   ├── resource/           # Game textures, sound effects, and Tiled maps
│   └── main.cpp            # Application entry point
└── DQGameEngine.sln        # Visual Studio 2022 Solution
