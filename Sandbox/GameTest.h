#pragma once
#include "DQEngine.h"	
#include "Background.h"

class GameTest : public GameApp
{
public:
	GameTest(int width, int height, const char* title) : GameApp(width, height, title) {}

	

	void OnLoad() override;
	void OnUpdate(float dt) override;
};

