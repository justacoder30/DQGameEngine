#include "pch.h"
#include "GameApp.h"
#include "Renderer/Renderer2D.h"
#include "Core/Input.h" 

Boardphase GameApp::s_Boardphase;
bool m_Running = true;

GameApp::GameApp(int width, int height, const char* title)
{
	Renderer2D::InitWindow(width, height, title);
    Renderer2D::InitRenderer();
    Renderer2D::SetViewport(width, height);
    

    //SDL_GL_SetSwapInterval(true);

    m_LastTime = SDL_GetPerformanceCounter();
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
}

GameApp::~GameApp()
{
	Renderer2D::Destroy();
}

void GameApp::Run()
{
    SDL_GL_SetSwapInterval(false);
	Load();
    GameLoop();
}

Boardphase* GameApp::GetBoardphase()
{
    return &s_Boardphase;
}

void GameApp::GameLoop()
{
    while (!Input.Quit())
    {
        Input.Update();


        Uint64 current = SDL_GetPerformanceCounter();
        float dt = (float)(current - m_LastTime) / SDL_GetPerformanceFrequency();
        if (dt > 1 / 60.f) dt = 1 / 60.f;
		//std::cout << "FPS: " << 1 / dt << std::endl;    
        m_LastTime = current;

        GameApp::GetBoardphase()->Run();
        Update(dt);

		Draw();
    }
}

void GameApp::Draw()
{
    Component::Draw();

    Renderer2D::BeginScene();

    Renderer2D::StartBatch();
    Renderer2D::FlushLayer(RenderLayer::Background);
    Renderer2D::EndBatch();

    Renderer2D::StartBatch(Renderer2D::GetCamera());
	Renderer2D::FlushLayer(RenderLayer::World);
    Renderer2D::EndBatch();

    Renderer2D::StartBatch();
    Renderer2D::FlushLayer(RenderLayer::UI);
    Renderer2D::EndBatch();

    Renderer2D::EndScene();

	Renderer2D::ClearCommandQueue();
}
