#pragma once
#include "PositionComponent.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer2D.h"

namespace dqengine {

class SpriteComponent : public PositionComponent
{
public:
    SpriteComponent(const std::string& path);
    SpriteComponent();
	void SetColor(const Color& color) { m_Color = color; }
protected:

    void OnDraw() override;

private:

    SharedPtr<Texture> m_Texture;
    Color m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
};
} // namespace dqengine
