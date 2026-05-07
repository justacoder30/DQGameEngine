#include "pch.h"
#include "PositionComponent.h"
#include "Renderer/Renderer2D.h"

void PositionComponent::OnUpdate(float dt)
{
	//if (layer == RenderLayer::UI) {
 //       dst.x = position.x - anchor.x * size.x;
 //       dst.y = position.y - anchor.y * size.y;
 //       dst.w = size.x;
 //       dst.h = size.y;
 //       return;
 //   }

    if (flip == Flip::Horizontal || flip == Flip::Diagonal) {
        dst.x -= size.x * (1.0f - anchor.x * 2.0f);
    }

    if (flip == Flip::Vertical || flip == Flip::Diagonal) {
        dst.y -= size.y * (1.0f - anchor.y * 2.0f);
    }

    Vector worldPos = GetWorldPosition();

    dst.x = worldPos.x - anchor.x * size.x;
    dst.y = worldPos.y - anchor.y * size.y;

	dst = Rect(dst.x, dst.y, size.x, size.y);
}

void PositionComponent::OnDraw() 
{
    if (!m_Visible)
        return;

    Component::OnDraw();
}

void PositionComponent::OnDetach()
{
    auto camera = Renderer2D::GetCamera();
    if(camera && camera->GetTarget() == this) {
        camera->Follow(nullptr);
	}
}

const Vector& PositionComponent::GetWorldPosition()
{
    Vector worldPos = position;

    Component* p = GetParent();
    auto t = dynamic_cast<PositionComponent*>(p);
    if (t) {
        worldPos.x += t->dst.x;
        worldPos.y += t->dst.y;
    }
    return worldPos;
}

void PositionComponent::SetPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

void PositionComponent::SetAnchor(float x, float y)
{
	anchor.x = x;
	anchor.y = y;
}

void PositionComponent::HorizontalFlip()
{
    switch (flip) {
        case None:
            flip = Horizontal;
            break;
        case Horizontal:
            flip = None;
            break;
        case Vertical:
            flip = Diagonal;
            break;
        case Diagonal:
            flip = Vertical;
            break;
    }
    anchor.x = 1.0f - anchor.x;
}

void PositionComponent::VerticalFlip()
{
    switch (flip) {
        case None:
            flip = Vertical;
            break;
        case Horizontal:
            flip = Diagonal;
            break;
        case Vertical:
            flip = None;
            break;
        case Diagonal:
            flip = Horizontal;
            break;
    }
    
    anchor.y = 1.0f - anchor.y;
}
