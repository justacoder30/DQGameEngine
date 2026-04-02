#include "pch.h"
#include "CollisionSystem.h"

void CollisionSystem::Add(ShapeComponent* c)
{
	colliders.push_back(c);
}

void CollisionSystem::Run()
{
    for (int i = 0; i < colliders.size(); i++)
    {
        for (int j = i + 1; j < colliders.size(); j++)
        {
            auto a = colliders[i];
            auto b = colliders[j];

            if (a->CheckCollide(b))
            {
                HandleCollision(a, b);
            }
        }
    }
}

void CollisionSystem::HandleCollision(ShapeComponent* a, ShapeComponent* b)
{
    auto ca = dynamic_cast<CollisionCallbacks*>(a->GetParent());
    auto cb = dynamic_cast<CollisionCallbacks*>(b->GetParent());

    if (ca) ca->OnCollision(b, b->GetParent());
    if (cb) cb->OnCollision(a, a->GetParent());
}

void CollisionSystem::Remove(ShapeComponent* c)
{
    colliders.erase(
        std::remove(colliders.begin(), colliders.end(), c),
        colliders.end()
    );
}
