#include "pch.h"
#include "SpriteComponent.h"
#include "Core/TextureManager.h"

SpriteComponent::SpriteComponent(const std::string& path)
{
	m_Texture = TextureManager::Load(path);
	position.x = 0, position.y = 0;
	size.x = m_Texture->GetWidth() / 3, size.y = m_Texture->GetHeight() / 3;
}

void SpriteComponent::OnDraw()
{
	Rect m_Src(0, 0, m_Texture->GetWidth(), m_Texture->GetHeight());

	if (layer == RenderLayer::World && !Renderer2D::GetCamera()->CanSee(dst))
		return;

	Renderer2D::Submit({
		CommandType::Sprite,
		m_Texture,
		m_Src,
		dst,
		flip,
		angle,
		anchor,
		layer
	});
}
