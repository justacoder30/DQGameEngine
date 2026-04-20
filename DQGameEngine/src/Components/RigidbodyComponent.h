#pragma once
#include "Components/Component.h"
#include "Core/Vector.h"

class RigidbodyComponent : public Component
{
public:

    

    void SetJump(float jumpHeight, float jumpTime)
    {
        gravity = (2 * jumpHeight) / (jumpTime * jumpTime);
        jump = 2 * jumpHeight / jumpTime;
	}

    float gravity = 0.f;
    float jump = 0.f;
};

	
