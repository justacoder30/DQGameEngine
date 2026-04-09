#pragma once
#include "Components/Component.h"
#include "Core/Vector.h"

enum class BodyType
{
    Static,
    Dynamic,
    Kinematic
};

class RigidbodyComponent : public Component
{
public:
    Vector velocity;
    float gravityScale = 1.0f;
    bool useGravity = true;
    BodyType bodyType = BodyType::Dynamic;

    void SetJump(float jumpHeight, float jumpTime)
    {
        gravity = (2 * jumpHeight) / (jumpTime * jumpTime);
        jump = 2 * jumpHeight / jumpTime;
	}

    float gravity = 0.f;
    float jump = 0.f;
};

	
