#pragma once
#include "Renderer/Texture.h"

class Animation
{
public:
	Texture* texture;
	float FrameCount;
	float FrameSpeed;
	bool loop;
	int CurrentFrame;
	int FrameWidth;
	int FrameHeight;

	Animation();
	Animation(const char* f_p, int frame_count, float frame_speed = 0.08, bool Loop = true);
	int LastFrame();
};