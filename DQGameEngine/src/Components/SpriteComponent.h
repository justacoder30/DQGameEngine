#pragma once
#include "Component.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer2D.h"

class SpriteComponent : public Component
{
public:
    float x, y, width, height;

    SpriteComponent(Texture* tex);

protected:

    void OnDraw() override;

private:

    Texture* m_Texture;
	
};