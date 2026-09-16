#pragma once
#include "Components/ShapeComponent.h"

class Board
{
public:
	virtual std::vector<ShapeComponent*> Query(ShapeComponent* obj) = 0;
	virtual void Insert(ShapeComponent* obj) = 0;
	virtual void Clear() = 0;
};

