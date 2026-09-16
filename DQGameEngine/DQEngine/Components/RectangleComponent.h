#pragma once
#include "ShapeComponent.h"
#include "Core/Rect.h"	

class RectangleComponent : public ShapeComponent
{
public:
	RectangleComponent();
	RectangleComponent(const Vector& position, const Vector& size);

	Rect GetWorldBounds();
    bool CheckCollide(ShapeComponent* other) override;
	Rect GetBounds() const { return bounds; }

	void OnDraw() override;
	void OnAttach() override;

private:
	Rect bounds;
	bool fixParentSize = false;
};

