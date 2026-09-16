#include "pch.h"

#include "Broadphase.h"
#include "BaseBoard.h"
#include "Components/RectangleComponent.h"

#include <algorithm>
#include <functional>

Boardphase::Boardphase() : board(std::make_unique<BaseBoard>())
{
    colliders.reserve(256);
    currentCollisions.reserve(256);
    previousCollisions.reserve(256);
}

void Boardphase::Add(ShapeComponent* c)
{
    if (!c) return;

    colliders.push_back(c);
}

void Boardphase::Run()
{
    currentCollisions.clear();

    board->Clear();

    for (ShapeComponent* c : colliders)
    {
        if (!c) continue;
        if (!c->active) continue;
        if (!c->GetParent()) continue;

        board->Insert(c);
    }
   
    // Collision detection
    for (ShapeComponent* a : colliders)
    {
        if (!a) continue;
        if (!a->active) continue;
        if (!a->GetParent()) continue;

        auto neighbors = board->Query(a);

        for (ShapeComponent* b : neighbors)
        {
            if (!b) continue;
            if (!b->active) continue;
            if (!b->GetParent()) continue;

            if (!std::less<ShapeComponent*>{}(a, b)) continue;
            if (!ShouldCollide(a, b)) continue;
            if (!a->CheckCollide(b)) continue;

            const CollisionPair pair = MakePair(a, b);

            currentCollisions.insert(pair);

            if (previousCollisions.contains(pair)) HandleCollision(a, b);
            else HandleCollisionStart(a, b);
        }
    }

    for (const CollisionPair& pair : previousCollisions)
    {
        if (!currentCollisions.contains(pair))
            HandleCollisionEnd( pair.first, pair.second);
    }

    
    currentCollisions.swap(previousCollisions);
    //previousCollisions = currentCollisions;
}

void Boardphase::HandleCollision( ShapeComponent* a, ShapeComponent* b)
{
    if (a->callback) {
        a->callback->OnCollision(a, b, b->GetParent());
    }

    if (b->callback) {
        b->callback->OnCollision(b, a, a->GetParent());
    }
}

void Boardphase::HandleCollisionStart(ShapeComponent* a, ShapeComponent* b)
{
    if (a->callback) {
        a->callback->OnCollisionStart(a, b, b->GetParent());
    }

    if (b->callback) {
        b->callback->OnCollisionStart(b, a, a->GetParent());
    }
}

void Boardphase::HandleCollisionEnd(ShapeComponent* a, ShapeComponent* b)
{
    if (a->callback) {
        a->callback->OnCollisionEnd(a, b, b->GetParent());
    }

    if (b->callback) {
        b->callback->OnCollisionEnd(b, a, a->GetParent());
    }
}

void Boardphase::Remove(ShapeComponent* c)
{
    if (!c) return;

    std::erase(colliders, c);

    auto removeCollider = [c](auto& collisions)
    {
        for (auto it = collisions.begin(); it != collisions.end();)
        {
            if (it->first == c || it->second == c) {
                it = collisions.erase(it);
            } else ++it;
        }
    };

    removeCollider(currentCollisions);
    removeCollider(previousCollisions);
}

CollisionPair Boardphase::MakePair(ShapeComponent* a, ShapeComponent* b) const
{
    if (std::less<ShapeComponent*>{}(a, b))
        return { a, b };

    return { b, a };
}

bool ShouldCollide(ShapeComponent* a, ShapeComponent* b)
{
    const bool aCanHitB = (a->mask & ToMask(b->layer)) != 0;

    const bool bCanHitA = (b->mask & ToMask(a->layer)) != 0;

    return aCanHitB && bCanHitA;
} 