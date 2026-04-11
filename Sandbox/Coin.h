#pragma once
#include "DQEngine.h"
#include <iostream>

class Coin : public Animation2DComponent, public CollisionCallbacks
{
public:
	Coin(float x,  float y);
	void OnLoad() override;
};

