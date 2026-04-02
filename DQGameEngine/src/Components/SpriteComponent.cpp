#include "pch.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(Texture* tex) : m_Texture(tex)
{
	position.x = 0, position.y = 0;
	size.x = tex->GetWidth()/3, size.y = tex->GetHeight()/3;
}

void SpriteComponent::OnDraw()
{
	Vector anchorOffset(size.x * anchor.x, size.y * anchor.y);

	Rect m_Src(0, 0, m_Texture->GetWidth(), m_Texture->GetHeight());
	Rect m_Dst(position.x - anchorOffset.x, position.y - anchorOffset.y, size.x, size.y);

	Renderer2D::Draw(*m_Texture, m_Src, m_Dst);
}
