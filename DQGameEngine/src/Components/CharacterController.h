#pragma once
#include "Components/Component.h"
#include "Core/Vector.h"
#include "Core/Broadphase.h"

class ShapeComponent;

class CharacterController : public Component
{
public:
    Vector velocity;

    float gravity = 0.f;
    float jumpForce = 0.f;

    ShapeComponent* collider = nullptr;

	void OnUpdate(float dt) override;   
	void OnLoad() override; 

private:
    void MoveX(float dx);
    void MoveY(float dy);

	std::vector<ShapeComponent*> colliders;
	Board* board;
};