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

private:
    std::vector<ShapeComponent*> colliders;
};

