#pragma once
#include "Animation.h"
#include "Core/Rect.h"
#include "Core/Flip.h"

class AnimationClip
{
private:
	float timer = 0;
	int step = 0;
	float deltaTime = 0;
public:
	Animation animation;
	bool loop;

	AnimationClip();
	AnimationClip(Animation Animation);
	void Play(Animation _animation);
	void ResetFrame();
	void Update(const float& dt);
	void stop();
	bool IsDone();
	Rect getRect();
};

