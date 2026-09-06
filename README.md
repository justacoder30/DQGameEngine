# DQGameEngine 🎮

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3%2B-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20x64-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

**DQGameEngine** is a custom 2D C++ game engine designed for educational and performance-focused game development. The repository includes both the core engine architecture and a **Sandbox** project demonstrating the engine's capabilities.

---

## 🌟 Features

- **2D Rendering Engine:** Modern OpenGL pipeline with Glad loader, supporting 2D sprite batching, custom shader management, and texture atlases.
- **2D Camera System:** Dynamic 2D camera supporting target tracking, smooth follow (Lerp), zoom controls, and viewport boundary clamping.
- **Animation System:** Frame-based sprite-sheet animation manager with configurable playback rates, integrated with a Finite State Machine (FSM) for character state transitions.
- **Collision Detection & Spatial Partitioning:** Axis-Aligned Bounding Box (AABB) collision detection and resolution algorithms, optimized with a **Spatial Grid** for fast broad-phase collision filtering and dynamic entity partitioning.
- **Windowing & Input:** Powered by **SDL3** for cross-platform window handling, graphics context creation, and real-time event processing.
- **Audio & Media:** Multi-channel sound effects and background music management via `SDL3_mixer`, font rendering with `SDL3_ttf`, and texture loading with `SDL3_image`.
- **Tilemap Support:** Native `.tmx` Tiled map parsing and multi-layer rendering integrated with **tmxlite**.
- **Math Library:** Hardware-accelerated vector and matrix calculations powered by **GLM**.

---
![Sandbox Demo](.github/assets/Picture1(1).png)
