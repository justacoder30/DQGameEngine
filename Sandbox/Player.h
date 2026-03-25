#pragma once
#include "DQEngine.h"
class Player : public Animation2DComponent
{
public:
	SpriteComponent* sprite1;

	Player();	

	void OnUpdate(float dt) override;
};

