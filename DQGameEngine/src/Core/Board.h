#pragma once
#include "Components/ShapeComponent.h"

class Board
{
public:
	virtual std::vector<ShapeComponent*> Query(ShapeComponent* obj) {
		std::vector<ShapeComponent*> result;
		return result;
	};
	virtual void Insert(ShapeComponent* obj) {};
	virtual void Clear() {};
};

