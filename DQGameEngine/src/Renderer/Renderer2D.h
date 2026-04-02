#pragma once
#include <SDL3/SDL.h>
#include "Texture.h"
#include "Core/Rect.h" 
#include "Core/Vector.h" 
#include "Core/Flip.h"
#include <glm/glm.hpp>

struct QuadVertex
{
    float Position[3];
    float TexCoord[2];
    int   TexIndex;
};

class Renderer2D
{
public:

    static void InitWindow(float windowWidth, float windowHeight, const char* title);
    static void InitRenderer();
    static void BeginScene();
    static void EndScene();
	static void Destroy();
    static void SetViewport(float gameWidth, float gameHeight);
    static void Draw(Texture& texture, Rect srcrect, Rect dstrect, Flip flip = None, float angle = 0.0f, Vector centerP = Vector::Zero());
    static void DrawRect(const Rect& rect);
    static void DrawRectOutline(const Rect& rect, float thickness = 2.0f);
	static Vector GetViewportSize() { return Vector(gameWidth, gameHeight); }
    static void SetCamera(const glm::mat4& viewProj);

private:

    static void Flush();
    static void StartBatch();
    static void NextBatch();

    static float m_WindowWidth;
    static float m_WindowHeight;
    static float gameWidth;
    static float gameHeight;
    static SDL_Window* m_Window;
    static SDL_GLContext m_Context;
};