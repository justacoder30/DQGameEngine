#include "pch.h"
#include "PositionComponent.h"

Vector PositionComponent::GetWoldPosition()
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
