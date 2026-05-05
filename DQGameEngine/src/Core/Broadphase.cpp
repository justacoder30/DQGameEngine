#include "pch.h"
#include "Broadphase.h"
#include "BaseBoard.h"
#include "Components/RectangleComponent.h"
#include "Components/RigidbodyComponent.h"

Boardphase::Boardphase()
{
	board = new BaseBoard();
}

void Boardphase::Add(ShapeComponent* c)
{
	colliders.push_back(c);
}

void Boardphase::Run()
{
    currentCollisions.clear();
    board->Clear();

    for (auto* c : colliders)
        board->Insert(c);

    for (auto* a : colliders)
    {
        if (!a->active) continue;
        auto neighbors = board->Query(a);

        for (auto* b : neighbors)
        {
            if (a >= b) continue;

            if (!b->active) continue;

            if (!a->GetParent() || !b->GetParent())
                continue;

            if (!ShouldCollide(a, b))
                continue;

            if (a->CheckCollide(b))
            {
                auto pair = MakePair(a, b);

                currentCollisions.insert(pair);

                if (previousCollisions.count(pair)) HandleCollision(a, b);
                else HandleCollisionStart(a, b);
            }
        }
    }

    for (auto& pair : previousCollisions)
    {
        if (!currentCollisions.count(pair))
            HandleCollisionEnd(pair.first, pair.second);
    }

    previousCollisions = currentCollisions;
}

void Boardphase::HandleCollision(ShapeComponent* a, ShapeComponent* b)
{
    if (a->callback)
        a->callback->OnCollision(a, b, b->GetParent());

    if (b->callback)
        b->callback->OnCollision(b, a, a->GetParent());
}

void Boardphase::HandleCollisionStart(ShapeComponent* a, ShapeComponent* b)
{
    if (a->callback)
        a->callback->OnCollisionStart(a, b, b->GetParent());

    if (b->callback)
        b->callback->OnCollisionStart(b, a, a->GetParent());
}

void Boardphase::HandleCollisionEnd(ShapeComponent* a, ShapeComponent* b)
{
    if (a->callback)
        a->callback->OnCollisionEnd(a, b, b->GetParent());

    if (b->callback)
        b->callback->OnCollisionEnd(b, a, a->GetParent());
}

void Boardphase::Remove(ShapeComponent* c)
{
    colliders.erase(
        std::remove(colliders.begin(), colliders.end(), c),
        colliders.end()
    );
    auto removeIfContains = [&](auto& set)
    {
        for (auto it = set.begin(); it != set.end(); )
        {
            if (it->first == c || it->second == c)
                it = set.erase(it);
            else
                ++it;
        }
    };

    removeIfContains(currentCollisions);
    removeIfContains(previousCollisions);
}

std::pair<ShapeComponent*, ShapeComponent*> Boardphase::MakePair(ShapeComponent* a, ShapeComponent* b)
{
    if (a < b)
        return { a, b };
    return { b, a };
}

bool ShouldCollide(ShapeComponent* a, ShapeComponent* b)
{
    bool aCanHitB = (a->mask & ToMask(b->layer)) != 0;
    bool bCanHitA = (b->mask & ToMask(a->layer)) != 0;

    return aCanHitB && bCanHitA;
}
