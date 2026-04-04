#include "pch.h"
#include "Animation.h"
#include "Core/TextureManager.h"

Animation::Animation()
{
}

Animation::Animation(const char* f_p, int frame_count, float frame_speed, bool Loop)
{
	texture = TextureManager::Load(f_p);
	FrameCount = frame_count;
	FrameSpeed = frame_speed;
	loop = Loop;
	CurrentFrame = 0;
	FrameWidth = texture->GetWidth() / FrameCount;
	FrameHeight = texture->GetHeight();
}

int Animation::LastFrame()
{
	return FrameCount - 1;
}
