#include "pch.h"
#include "RectangleComponent.h"
#include "Renderer/Renderer2D.h"

RectangleComponent::RectangleComponent()
{
    fixParentSize = true;
}

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

	auto rectOther = static_cast<RectangleComponent*>(other);
	bool collided = GetWorldBounds().CheckCollide(rectOther->GetWorldBounds());
    
    if (collided) {
        isColliding = true;
		other->isColliding = true;
    }

    return collided;
}

void RectangleComponent::OnDraw()
{
    if (!DebugMode) return;

	Renderer2D::DrawRectOutline(GetWorldBounds(), debugColor, 1.f);
}

void RectangleComponent::OnAttach()
{
    if (fixParentSize) {
        auto parent = dynamic_cast<PositionComponent*>(GetParent());
        if (parent) {
            bounds = Rect(Vector::Zero(), parent->size);
            shapeType = ShapeType::Rectangle;
        }
    }
	
    ShapeComponent::OnAttach();
}
