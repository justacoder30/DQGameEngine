#pragma once
#include "DQEngine.h"
#include "Coin.h"
#include <iostream>

class Background : public PositionComponent
{

public:
	Background() {}

	void OnLoad() override;
	void OnUpdate(float dt) override;

private:
	float cloud_speed = 15.f;
	std::vector<SpriteComponent*> clouds;
};

