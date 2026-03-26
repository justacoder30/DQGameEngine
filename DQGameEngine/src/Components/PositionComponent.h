#pragma once
#include "Component.h"
#include "Core/Vector.h"

class PositionComponent : public Component
{
public:

	PositionComponent() : position(0), size(0) {}
	PositionComponent(const Vector& position, const Vector& size = 0) : position(position), size(size) {}	
	
	Vector position;
	Vector size;
};

