#pragma once
#include "DQEngine.h"

class Player : public Animation2DComponent, public CollisionCallbacks
{
public:
	Player();	

	void OnUpdate(float dt) override;
	void OnCollision(ShapeComponent* shape, Component* other) override;
};

