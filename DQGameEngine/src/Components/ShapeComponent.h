#pragma once
#include "PositionComponent.h"

enum Layer
{
	Empty = 0,
	Player = 1 << 0,
	Enemy = 1 << 1,
	Ground = 1 << 2,
	Attack = 1 << 3,
	Sensor = 1 << 4,
	Item = 1 << 5,
};

class ShapeComponent : public PositionComponent
{
public:

	ShapeComponent(){}
	ShapeComponent(const Vector& position, const Vector& size) : PositionComponent(position, size) {}
	
	virtual bool CheckCollide(ShapeComponent* other) { return false; }
	
	void OnAttach() override;
	void OnDetach() override;

	uint32_t mask = 0;
	Layer layer = Layer::Empty;
	bool isTrigger = false;
	bool hasPhysics = true;
};

