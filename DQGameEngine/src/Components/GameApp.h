#pragma once

#include "Component.h"
#include <SDL3/SDL.h>
#include "Core/CollisionSystem.h"

class GameApp : public Component
{
public:
    GameApp(int width, int height, const char* title);
    ~GameApp();

    void Run();

    static CollisionSystem* GetCollisionSystem();

protected:

    void GameLoop();
    void OnUpdate(float dt) override {}
    void OnDraw() override {}

private:
    static CollisionSystem s_CollisionSystem;
    SDL_Window* m_Window = nullptr;
    SDL_GLContext m_Context = nullptr;

    bool m_Running = true;

    Uint64 m_LastTime = 0;
};