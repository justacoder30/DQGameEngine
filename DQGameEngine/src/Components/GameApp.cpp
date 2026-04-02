#include "pch.h"
#include "GameApp.h"
#include "Renderer/Renderer2D.h"
#include "Core/Input.h" 

CollisionSystem GameApp::s_CollisionSystem;

GameApp::GameApp(int width, int height, const char* title)
{
	Renderer2D::InitWindow(width, height, title);
    Renderer2D::InitRenderer();
    Renderer2D::SetViewport(width, height);

    m_LastTime = SDL_GetPerformanceCounter();
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
}

GameApp::~GameApp()
{
	Renderer2D::Destroy();
}

void GameApp::Run()
{
	Load();
    GameLoop();
}

CollisionSystem* GameApp::GetCollisionSystem()
{
    return &s_CollisionSystem;
}

void GameApp::GameLoop()
{
    while (m_Running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                m_Running = false;
        }

        Input.Update();


        Uint64 current = SDL_GetPerformanceCounter();
        float dt = (float)(current - m_LastTime) / SDL_GetPerformanceFrequency();
        if (dt > 1 / 60.f) dt = 1 / 60.f;
        m_LastTime = current;

        Update(dt);
        GameApp::GetCollisionSystem()->Run();

        Renderer2D::BeginScene();
        Draw();
        Renderer2D::EndScene();

        
    }
}
