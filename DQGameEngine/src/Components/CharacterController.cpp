#include "pch.h"
#include "CharacterController.h"
#include "Core/Input.h"
#include "Components/PositionComponent.h"
#include "Components/GameApp.h"
#include "Components/RectangleComponent.h"
#include "Components/RigidbodyComponent.h"

void CharacterController::OnLoad()
{
    colliders = GameApp::GetBoardphase()->GetColliders();
    board = GameApp::GetBoardphase()->GetBoard();
}

void CharacterController::OnUpdate(float dt)
{
    if (!hitbox) return;

 //   if(velocity.x != 0) MoveX(velocity.x * dt);
	//if(velocity.y != 0) MoveY(velocity.y * dt);

    Vector move = velocity * dt;

    int steps = (int)(std::max(abs(move.x), abs(move.y)) / StepSize) + 1;

    for (int i = 0; i < steps; i++)
    {
        if (velocity.x == 0 && velocity.y == 0)
            break;

        Vector stepMove = move / (float)steps;

        if (stepMove.x != 0) MoveX(stepMove.x);
        if (stepMove.y != 0) MoveY(stepMove.y);
    }
}

void CharacterController::MoveX(const float& dx)
{
	auto pos = static_cast<PositionComponent*>(GetParent());
    if (!hitbox) return;

    pos->position.x += dx;

    auto neighbors = board->Query(hitbox);

    for (auto* neighbor : neighbors)
    {
        if (hitbox == neighbor)
            continue;

        if (!hitbox->active || !neighbor->active)
            continue;

        if (neighbor->bodyType == BodyType::NoneType)
            continue;

        if (!ShouldCollide(hitbox, neighbor))
            continue;

        if (hitbox->CheckCollide(neighbor))
        {
            auto ra = static_cast<RectangleComponent*>(hitbox);
            auto rb = static_cast<RectangleComponent*>(neighbor);

            Vector mtv = ra->GetWorldBounds().GetMTV(rb->GetWorldBounds());

            //pos->position.x += mtv.x;

            if (neighbor->bodyType == BodyType::Dynamic)
            {
                std::unordered_set<ShapeComponent*> visited;
				visited.insert(hitbox);
                if (!Push(neighbor, mtv, visited))
                {
                    pos->position.x += mtv.x;
                    velocity.x = 0;
                }
            }
            else if (neighbor->bodyType == BodyType::Static)
            {
                pos->position.x += mtv.x;
                velocity.x = 0;
            }
        }
    }
}

void CharacterController::MoveY(const float& dy)
{
	auto pos = static_cast<PositionComponent*>(GetParent());
    if (!hitbox) return;

    pos->position.y += dy;

    auto neighbors = board->Query(hitbox);
    for (auto* neighbor : neighbors)
    {
        if (hitbox == neighbor) 
            continue;

        if (!hitbox->active || !neighbor->active)
            continue;

		if (neighbor->bodyType == BodyType::NoneType)
            continue;

        if (!ShouldCollide(hitbox, neighbor))
            continue;

        if (hitbox->CheckCollide(neighbor))
        {
            auto ra = static_cast<RectangleComponent*>(hitbox);
            auto rb = static_cast<RectangleComponent*>(neighbor);

            Vector mtv = ra->GetWorldBounds().GetMTV(rb->GetWorldBounds());

            //pos->position.y += mtv.y;

            if (neighbor->bodyType == BodyType::Dynamic)
            {
                std::unordered_set<ShapeComponent*> visited;
                visited.insert(hitbox);
                if (!Push(neighbor, mtv, visited))
                {
                    pos->position.y += mtv.y;
                    velocity.y = 0;
                }
            }
            else if (neighbor->bodyType == BodyType::Static)
            {
                pos->position.y += mtv.y;
                velocity.y = 0;
            }

            
        }
    }
}

bool CharacterController::Push(ShapeComponent* target, const Vector& mtv, std::unordered_set<ShapeComponent*>& visited)
{
    if (visited.count(target))
        return true; 

    visited.insert(target);

    auto neighbors = board->Query(target);

    for (auto* n : neighbors)
    {
        if (n == target) continue;
        if (!ShouldCollide(target, n))
            continue;

        if (target->CheckCollide(n))
        {
            if (n->bodyType == BodyType::Static)
                return false;

            if (n->bodyType == BodyType::Dynamic)
            {
                if (!Push(n, mtv, visited))
                    return false;
            }
        }
    }


    auto p = static_cast<PositionComponent*>(target->GetParent());
    p->position -= mtv;

    return true;
}
