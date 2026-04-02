#pragma once
#include "PositionComponent.h"
class ShapeComponent : public PositionComponent
{
public:

	ShapeComponent(){}
	ShapeComponent(const Vector& position, const Vector& size) : PositionComponent(position, size) {}
	
	virtual bool CheckCollide(ShapeComponent* other) { return false; }
	
	void OnAttach() override;
	void OnDetach() override;

	bool isTrigger = false;
};

