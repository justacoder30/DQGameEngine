#include "BaseBoard.h"
#include "pch.h"
#include "BaseBoard.h"
#include "Components/GameApp.h"

std::vector<ShapeComponent*> BaseBoard::Query(ShapeComponent* obj)
{
	return colliders;
}

void BaseBoard::Insert(ShapeComponent* obj)
{
	colliders.push_back(obj);
}

void BaseBoard::Clear()
{
	colliders.clear();
}
