#pragma once
#include "Components/CameraComponent.h"
#include <SDL3/SDL.h>
#include "Texture.h"
#include "Core/Rect.h" 
#include "Core/Vector.h" 
#include "Core/Flip.h"
#include "Core/Color.h"
#include <glm/glm.hpp>
#include "RenderCommand.h"
#include "vector"

struct QuadVertex
{
    float Position[3];
    float TexCoord[2];
    int   TexIndex;

    float Color[4];
};

class Renderer2D
{
public:

    static void InitWindow(float windowWidth, float windowHeight, const char* title);
    static void InitRenderer();
    static void BeginScene();
    static void EndScene();
    static void StartBatch(CameraComponent* camera = nullptr);
    static void EndBatch();
	static void Destroy();

	static Vector GetViewportSize() { return Vector(s_Camera->GetViewBounds().w, s_Camera->GetViewBounds().h); }
	static Vector GetWindowSize() { return Vector(m_WindowWidth, m_WindowHeight); }
    static void SetViewport(float gameWidth, float gameHeight);
    static void Draw(Texture& texture, const Rect& srcrect, const Rect& dstrect, const Color& color, const Flip& flip = None, const float& angle = 0.0f, const Vector& centerP = Vector::Zero());
    static void DrawRect(const Rect& rect);
    static void DrawRectOutline(const Rect& rect, float thickness = 2.0f);
    static void SetMatrix(const glm::mat4& viewProj);
	static void SetCamera(CameraComponent* camera) { s_Camera = camera; }
	static CameraComponent* GetCamera() { return s_Camera; }

    static void Submit(const RenderCommand& cmd);
    static void SubmitRect(const Rect& rect, RenderLayer layer);
	static void FlushLayer(const RenderLayer& layer);
    static void ClearCommandQueue();
    static std::vector<RenderCommand>& GetQueue(const RenderLayer& layer);
    
    static SDL_Window* m_Window;
    
private:

    static void Flush();
    static void NextBatch();

    static CameraComponent* s_Camera;
    static float m_WindowWidth;
    static float m_WindowHeight;
    
    static SDL_GLContext m_Context;
    static std::vector<RenderCommand> s_BackgroundQueue;
    static std::vector<RenderCommand> s_WorldQueue;
    static std::vector<RenderCommand> s_UIQueue;
};