#include "pch.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip()
{}

AnimationClip::AnimationClip(Animation Animation)
{
	animation = Animation;
	loop = false;
}

void AnimationClip::Play(Animation _animation)
{
	animation = _animation;
	loop = animation.loop;
	ResetFrame();
}

void AnimationClip::ResetFrame()
{
	animation.CurrentFrame = 0;
	timer = 0;
}

void AnimationClip::Update(const float& dt)
{
	deltaTime = dt;
	timer += deltaTime;

	if (timer <= animation.FrameSpeed)
		return;

	timer = 0;
	animation.CurrentFrame += 1;

	if (animation.CurrentFrame >= animation.FrameCount) {
		if (animation.loop) animation.CurrentFrame = 0;
		else animation.CurrentFrame = animation.LastFrame();
	}
}

void AnimationClip::stop()
{
	animation.CurrentFrame = animation.LastFrame();
	loop = false;
	timer = 0;
}

bool AnimationClip::IsDone()
{
	if (animation.loop) return false;

	if (animation.CurrentFrame == animation.LastFrame() && timer + deltaTime > animation.FrameSpeed) {
		return true;
	}

	return false;
}

Rect AnimationClip::getRect()
{
	if(!animation.IsSheet()) return Rect(0, 0, animation.FrameWidth, animation.FrameHeight);
	return Rect(animation.CurrentFrame * animation.FrameWidth, animation.col * animation.FrameHeight, animation.FrameWidth, animation.FrameHeight);
}

