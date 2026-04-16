#include "pch.h"
#include "CharacterController.h"
#include "Core/Input.h"
#include "Components/PositionComponent.h"
#include "Components/GameApp.h"
#include "Components/RectangleComponent.h"
#include "Components/RigidbodyComponent.h"

void CharacterController::OnUpdate(float dt)
{
    MoveX(velocity.x * dt);
    MoveY(velocity.y * dt);
}

void CharacterController::OnLoad()
{
    colliders = GameApp::GetBoardphase()->GetColliders();
    board = GameApp::GetBoardphase()->GetBoard();
}

void CharacterController::MoveX(float dx)
{
    auto pos = static_cast<PositionComponent*>(GetParent());
    if (!collider) return;

    pos->position.x += dx;

    auto neighbors = board->Query(collider);

    for (auto* neighbor : neighbors)
    {
        if (!collider->active || !neighbor->active)
            continue;

        if (!GameApp::GetBoardphase()->ShouldCollide(collider, neighbor))
            continue;

        if (collider->CheckCollide(neighbor))
        {
            auto ra = static_cast<RectangleComponent*>(collider);
            auto rb = static_cast<RectangleComponent*>(neighbor);

            Vector mtv = ra->GetWorldBounds().GetMTV(rb->GetWorldBounds());

            //pos->position.x += mtv.x;

            auto rbOther = neighbor->GetParent()->GetComponent<RigidbodyComponent>();
            if (rbOther && rbOther->bodyType == BodyType::Dynamic)
            {
                auto p_neighbor = static_cast<PositionComponent*>(neighbor->GetParent());
                p_neighbor->position.x -= mtv.x;
            }
            else {
                pos->position.x += mtv.x;
            }

            velocity.x = 0;
        }
    }
}

void CharacterController::MoveY(float dy)
{
    auto pos = static_cast<PositionComponent*>(GetParent());
    if (!collider) return;

    pos->position.y += dy;

    auto neighbors = board->Query(collider);

    for (auto* neighbor : neighbors)
    {
        if (collider >= neighbor) continue;
        if (!GameApp::GetBoardphase()->ShouldCollide(collider, neighbor))
            continue;

        if (collider->CheckCollide(neighbor))
        {
            auto ra = static_cast<RectangleComponent*>(collider);
            auto rb = static_cast<RectangleComponent*>(neighbor);

            Vector mtv = ra->GetWorldBounds().GetMTV(rb->GetWorldBounds());

            //pos->position.y += mtv.y;

            auto rbOther = neighbor->GetParent()->GetComponent<RigidbodyComponent>();
            if (rbOther && rbOther->bodyType == BodyType::Dynamic)
            {
                auto p_neighbor = static_cast<PositionComponent*>(neighbor->GetParent());
                p_neighbor->position.y -= mtv.y;
            }
            else {
                pos->position.y += mtv.y;
                if (mtv.y != 0)
                {
                    velocity.y = 0;
                }
            }

            
        }
    }
}
