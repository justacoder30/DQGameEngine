#include "BaseBoard.h"
#include "pch.h"
#include "BaseBoard.h"
#include "Components/GameApp.h"

namespace dqengine {

void BaseBoard::Query(ShapeComponent* obj, std::vector<ShapeComponent*>& result)
{
	result.assign(colliders.begin(), colliders.end());
}

void BaseBoard::Insert(ShapeComponent* obj)
{
	colliders.push_back(obj);
}

void BaseBoard::Clear()
{
	colliders.clear();
}

void BaseBoard::Remove(ShapeComponent* obj)
{
    std::erase(colliders, obj);
}
} // namespace dqengine
