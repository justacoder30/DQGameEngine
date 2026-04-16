#pragma once
#include "Board.h"

class BaseBoard : public Board
{
public:
	std::vector<ShapeComponent*> Query(ShapeComponent* obj) override;
	void Insert(ShapeComponent* obj) override;
	void Clear() override;

private:
	std::vector<ShapeComponent*> colliders;
};

