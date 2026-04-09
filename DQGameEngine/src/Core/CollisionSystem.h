#pragma once
#include "Components/ShapeComponent.h"  
#include "CollisionCallbacks.h" 
#include <set>
#include <utility>

class CollisionSystem
{
public:

	CollisionSystem() = default;

    void Add(ShapeComponent* c);
    void Run();
    void HandleCollision(ShapeComponent* a, ShapeComponent* b);
    void HandleCollisionStart(ShapeComponent* a, ShapeComponent* b);
    void HandleCollisionEnd(ShapeComponent* a, ShapeComponent* b);
    bool ShouldCollide(ShapeComponent* a, ShapeComponent* b);
	void ResolveCollision(ShapeComponent* a, ShapeComponent* b);
    void Remove(ShapeComponent* c);

private:
    std::pair<ShapeComponent*, ShapeComponent*> MakePair(ShapeComponent* a, ShapeComponent* b);

    std::vector<ShapeComponent*> colliders;
    std::set<std::pair<ShapeComponent*, ShapeComponent*>> currentCollisions;
    std::set<std::pair<ShapeComponent*, ShapeComponent*>> previousCollisions;
};

