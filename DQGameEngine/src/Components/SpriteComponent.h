#pragma once
#include "PositionComponent.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer2D.h"

class SpriteComponent : public PositionComponent
{
public:
    SpriteComponent(const std::string& path);

protected:

    void OnDraw() override;

private:

    Texture* m_Texture;
	
};