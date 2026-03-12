#include "pch.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(Texture* tex) : m_Texture(tex) 
{
	x = 0, y = 0, width = tex->GetWidth(), height = tex->GetHeight();
}

void SpriteComponent::OnDraw()
{
	Rect m_Src(0, 0, m_Texture->GetWidth(), m_Texture->GetHeight());
	Rect m_Dst(x, y, width, height);

	Renderer2D::Draw(*m_Texture, m_Src, m_Dst);
}
