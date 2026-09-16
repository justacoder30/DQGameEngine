#pragma once

#include "Component.h"
#include <SDL3/SDL.h>
#include "Core/Broadphase.h"

class GameApp : public Component
{
public:
    GameApp(int width, int height, const char* title);
    ~GameApp();

    void Run();

    static Boardphase* GetBoardphase();

protected:

    void GameLoop();
    void OnUpdate(float dt) override {}
    void OnDraw() override {}
    void Draw();

    static Boardphase s_Boardphase;

private:
    Uint64 m_LastTime = 0;
};

extern bool m_Running;