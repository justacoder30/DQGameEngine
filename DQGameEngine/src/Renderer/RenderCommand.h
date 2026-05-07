#pragma once
#include "Texture.h"
#include "Core/Rect.h"
#include "Core/Flip.h"
#include "Core/Color.h"
#include <glm/glm.hpp>  

enum class CommandType
{
    Sprite,
    Rect
};

enum class RenderLayer
{
    Background,
    World,
    UI
};

struct RenderCommand
{
    CommandType type;

    Texture* texture;
    Rect src;
    Rect dst;
    Flip flip;
    float angle;
	Vector center;

    RenderLayer layer;

    Color color = { 1,1,1,1 };
};

