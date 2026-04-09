#pragma once
#include "DQEngine.h"

class Coin : public Animation2DComponent, public CollisionCallbacks
{
public:
	Coin(float x,  float y);
	void OnLoad() override;
	void OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
};

