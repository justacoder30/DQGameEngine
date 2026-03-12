#pragma once

#include "Component.h"
#include <SDL3/SDL.h>

class GameApp : public Component
{
public:
    GameApp(int width, int height, const char* title);
    ~GameApp();

    void Run();

protected:
    void OnUpdate(float dt) override;
    void OnDraw() override;

private:
    SDL_Window* m_Window = nullptr;
    SDL_GLContext m_Context = nullptr;

    bool m_Running = true;

    Uint64 m_LastTime = 0;
};