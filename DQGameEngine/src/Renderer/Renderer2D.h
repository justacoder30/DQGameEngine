#pragma once

#include "Texture.h"
#include "Core/Rect.h" 
#include "Core/Vector.h" 

struct QuadVertex
{
    float Position[3];
    float TexCoord[2];
    int   TexIndex;
};

class Renderer2D
{
public:

    static void Init();

    static void BeginScene();
    static void EndScene();

    static void SetViewport(float width, float height);

    static void Draw(Texture& texture, Rect srcrect, Rect dstrect, bool flip = false, float angle = 0.0f, Vector centerP = Vector::Zero());

private:
    static void Flush();
    static void StartBatch();
    static void NextBatch();
};