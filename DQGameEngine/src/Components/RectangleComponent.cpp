#include "pch.h"
#include "RectangleComponent.h"
#include "Renderer/Renderer2D.h"

RectangleComponent::RectangleComponent(const Vector& position, const Vector& size) : ShapeComponent(position, size)
{
    bounds = Rect(position, size);
}

Rect RectangleComponent::GetWorldBounds()
{
    Vector worldPos = position;

    Component* p = GetParent();
    while (p)
    {
        auto t = dynamic_cast<PositionComponent*>(p);
        if (t)
            worldPos += t->position;

        p = p->GetParent();
    }

    return Rect(worldPos, size);
}

bool RectangleComponent::CheckCollide(ShapeComponent* other)
{
    auto rectOther = dynamic_cast<RectangleComponent*>(other);

    if (!rectOther) return false;

    return GetWorldBounds().CheckCollide(rectOther->GetWorldBounds());
}

void RectangleComponent::OnDraw()
{
	Renderer2D::DrawRectOutline(GetWorldBounds());
}
