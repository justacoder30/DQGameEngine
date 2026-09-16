#pragma once
#include "Components/Component.h"
#include "Core/Vector.h"
#include "Core/Broadphase.h"
#include <unordered_set>

class ShapeComponent;

class CharacterController : public Component
{
public:
    Vector velocity;

    float gravity = 0.f;
    float jumpForce = 0.f;
	inline static float StepSize;

    ShapeComponent* hitbox = nullptr;

	void OnLoad() override; 
	void OnUpdate(float dt) override;
	void Move(const Vector& direction);
    void MoveX(const float& dx = 0);
    void MoveY(const float& dy = 0);

private:

    bool Push(ShapeComponent* target, const Vector& mtv, std::unordered_set<ShapeComponent*>& visited);

    std::vector<ShapeComponent*> colliders;
	Board* board;
};