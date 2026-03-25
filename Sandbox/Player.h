#pragma once
#include "DQEngine.h"
class Player : public Animation2DComponent
{
public:
	Player();	

	void OnUpdate(float dt) override;
};

