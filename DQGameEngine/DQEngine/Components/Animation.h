#pragma once
#include "Renderer/Texture.h"
#include "Core/Vector.h"

class Animation
{
public:
	float FrameCount;
	float FrameSpeed;
	bool loop;
	int CurrentFrame;
	int FrameWidth;
	int FrameHeight;

	Animation();
	Animation(const char* f_p, int frame_count, float frame_speed = 0.08, bool Loop = true);
	Animation(const char* f_p, int frame_count, const int& row, const Vector& Framesize, float frame_speed = 0.08, bool Loop = true);
	Animation(const std::vector<std::string>& paths, float frame_speed = 0.08, bool Loop = true);

	Texture* GetCurrentTexture();
	int LastFrame();
	bool IsSheet() { return isSpriteSheet; }	

	int row = 0;

private:
	std::vector<Texture*> textures;
	Texture* texture = nullptr;
	bool isSpriteSheet = true;
};