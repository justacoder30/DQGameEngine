#pragma once
#include "Components/ShapeComponent.h"  
#include "CollisionCallbacks.h" 

class CollisionSystem
{
public:

	CollisionSystem() = default;

    void Add(ShapeComponent* c);
    void Run();
    void HandleCollision(ShapeComponent* a, ShapeComponent* b);
    void Remove(ShapeComponent* c);

private:
    std::vector<ShapeComponent*> colliders;
};

