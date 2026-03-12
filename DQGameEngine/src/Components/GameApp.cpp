#include "pch.h"
#include "GameApp.h"
#include "Renderer/Renderer2D.h"
#include "Core/Input.h" 

GameApp::GameApp(int width, int height, const char* title)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_Window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);

    m_Context = SDL_GL_CreateContext(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to init GLAD\n";
    }

    Renderer2D::Init();
    Renderer2D::SetViewport(width, height);

    m_LastTime = SDL_GetPerformanceCounter();
}

GameApp::~GameApp()
{
    SDL_GL_DestroyContext(m_Context);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

void GameApp::Run()
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


        Renderer2D::BeginScene();
        Draw();
        Renderer2D::EndScene();

        SDL_GL_SwapWindow(m_Window);
    }
}

void GameApp::OnUpdate(float dt)
{
}

void GameApp::OnDraw()
{
}
