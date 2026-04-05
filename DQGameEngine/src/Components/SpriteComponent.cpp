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
	Vector anchorOffset(size.x * anchor.x, size.y * anchor.y);

	Rect m_Src(0, 0, m_Texture->GetWidth(), m_Texture->GetHeight());
	Rect m_Dst(position.x - anchorOffset.x, position.y - anchorOffset.y, size.x, size.y);

	Renderer2D::Submit({
		CommandType::Sprite,
		m_Texture,
		m_Src,
		m_Dst,
		flip,
		angle,
		Vector(0, 0),
		layer
	});
}
