#include "pch.h"
#include "PositionComponent.h"

Vector PositionComponent::GetWorldPosition()
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
}
