#include "pch.h"
#include "CollisionSystem.h"
#include "Components/RectangleComponent.h"
#include "Components/RigidbodyComponent.h"

void CollisionSystem::Add(ShapeComponent* c)
{
	colliders.push_back(c);
}

void CollisionSystem::Run()
{
    currentCollisions.clear();

    for (int i = 0; i < colliders.size(); i++)
    {
        for (int j = i + 1; j < colliders.size(); j++)
        {
            auto a = colliders[i];
            auto b = colliders[j];

            if (!a->GetParent() || !b->GetParent())
                continue;

            if (!ShouldCollide(a, b))
                continue;

            if (a->CheckCollide(b))
            {
                auto pair = MakePair(a, b);
                bool isTrigger = a->isTrigger || b->isTrigger;
                currentCollisions.insert(pair);

                if (previousCollisions.count(pair)) {
                    HandleCollision(a, b);
                } else {
                    HandleCollisionStart(a, b);
                }

                if (!isTrigger && (a->hasPhysics || b->hasPhysics))
                {
                    ResolveCollision(a, b);
                }
            }
        }
    }

    for (auto& pair : previousCollisions)
    {
        if (!currentCollisions.count(pair))
        {
            HandleCollisionEnd(pair.first, pair.second);
        }
    }

    previousCollisions = currentCollisions;
}

void CollisionSystem::HandleCollision(ShapeComponent* a, ShapeComponent* b)
{
    auto ca = dynamic_cast<CollisionCallbacks*>(a->GetParent());
    auto cb = dynamic_cast<CollisionCallbacks*>(b->GetParent());

	if (ca) ca->OnCollision(a, b, b->GetParent());
    if (cb) cb->OnCollision(b, a, a->GetParent());
}

void CollisionSystem::HandleCollisionStart(ShapeComponent* a, ShapeComponent* b)
{
    auto ca = dynamic_cast<CollisionCallbacks*>(a->GetParent());
    auto cb = dynamic_cast<CollisionCallbacks*>(b->GetParent());

    if (ca) ca->OnCollisionStart(a, b, b->GetParent());
    if (cb) cb->OnCollisionStart(b, a, a->GetParent());
}

void CollisionSystem::HandleCollisionEnd(ShapeComponent* a, ShapeComponent* b)
{
    auto ca = dynamic_cast<CollisionCallbacks*>(a->GetParent());
    auto cb = dynamic_cast<CollisionCallbacks*>(b->GetParent());
    if (ca) ca->OnCollisionEnd(a, b, b->GetParent());
	if (cb) cb->OnCollisionEnd(b, a, a->GetParent());
}

bool CollisionSystem::ShouldCollide(ShapeComponent* a, ShapeComponent* b)
{
    bool aCanHitB = (a->mask & ToMask(b->layer)) != 0;
    bool bCanHitA = (b->mask & ToMask(a->layer)) != 0;

    return aCanHitB && bCanHitA;
}

void CollisionSystem::ResolveCollision(ShapeComponent* a, ShapeComponent* b)
{
    auto ra = dynamic_cast<RectangleComponent*>(a);
    auto rb = dynamic_cast<RectangleComponent*>(b);

    if (!ra || !rb) return;

    Rect rectA = ra->GetWorldBounds();
    Rect rectB = rb->GetWorldBounds();

    Vector mtv = rectA.GetMTV(rectB);

    auto pa = dynamic_cast<PositionComponent*>(a->GetParent());
    auto pb = dynamic_cast<PositionComponent*>(b->GetParent());

    auto rba = a->GetParent()->GetComponent<RigidbodyComponent>();
    auto rbb = b->GetParent()->GetComponent<RigidbodyComponent>();


    if (!pa && !pb) return;

    bool isVertical = abs(mtv.y) < abs(mtv.x);

    if (rba && rba->bodyType == BodyType::Dynamic && (!rbb || rbb->bodyType == BodyType::Static))
    {
        pa->position += mtv;

        if (isVertical)
            rba->velocity.y = 0;
        else
            rba->velocity.x = 0;
    }
    else if (rbb && rbb->bodyType == BodyType::Dynamic && (!rba || rba->bodyType == BodyType::Static))
    {
        pb->position -= mtv;

        if (isVertical)
            rbb->velocity.y = 0;
        else
            rbb->velocity.x = 0;
    }
    else if (rba && rbb && rba->bodyType == BodyType::Dynamic && rbb->bodyType == BodyType::Dynamic)
    {
        pa->position += mtv * 0.5f;
        pb->position -= mtv * 0.5f;

        if (isVertical) {
            rba->velocity.y = 0;
            rbb->velocity.y = 0;
        }
        else {
            rba->velocity.x = 0;
            rbb->velocity.x = 0;
        }
    }
}

void CollisionSystem::Remove(ShapeComponent* c)
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

std::pair<ShapeComponent*, ShapeComponent*> CollisionSystem::MakePair(ShapeComponent* a, ShapeComponent* b)
{
    if (a < b)
        return { a, b };
    return { b, a };
}
