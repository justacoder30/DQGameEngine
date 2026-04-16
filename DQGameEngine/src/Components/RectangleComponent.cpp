#include "pch.h"
#include "RectangleComponent.h"
#include "Renderer/Renderer2D.h"

RectangleComponent::RectangleComponent(const Vector& position, const Vector& size) : ShapeComponent(position, size)
{
    bounds = Rect(position, size);
	shapeType = ShapeType::Rectangle;
}

Rect RectangleComponent::GetWorldBounds()
{
    Component* p = GetParent();
    auto t = dynamic_cast<PositionComponent*>(p);
	if (!t) return Rect(Vector(bounds.x, bounds.y), size);
    Vector spriteSize = t->size;
    Vector anchor = t->anchor;

    Vector pivot = t->GetWorldPosition();
    Flip flip = t->flip;

    float offsetX = bounds.x - anchor.x * spriteSize.x;
    float offsetY = bounds.y - anchor.y * spriteSize.y;

    if (flip == Flip::Horizontal || flip == Flip::Diagonal) {
        offsetX = bounds.x - (1 - anchor.x) * spriteSize.x;
        offsetX = -offsetX - bounds.w;
    }
        
    if (flip == Flip::Vertical || flip == Flip::Diagonal) {
        offsetY = bounds.y - (1 - anchor.y) * spriteSize.y;
        offsetY = -offsetY - bounds.h;
    }
        
    return Rect(
        pivot.x + offsetX,
        pivot.y + offsetY,
        bounds.w,
        bounds.h
    );
    
}

bool RectangleComponent::CheckCollide(ShapeComponent* other)
{
	if (other->shapeType != ShapeType::Rectangle) return false;

	auto rectOther = dynamic_cast<RectangleComponent*>(other);
    isColliding = GetWorldBounds().CheckCollide(rectOther->GetWorldBounds());

    return isColliding;
}

void RectangleComponent::OnDraw()
{
	Renderer2D::DrawRectOutline(GetWorldBounds(), 1.f);
}
