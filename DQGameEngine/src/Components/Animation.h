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
	Animation(const char* f_p, int frame_count, float frame_speed = 0.08, bool Loop = true, const int& col = 0, const int& FrameHeightCount = 1, const Vector& Framesize = Vector::Zero());
	Animation(const std::vector<std::string>& paths, float frame_speed = 0.08, bool Loop = true);

	Texture* GetCurrentTexture();
	int LastFrame();
	bool IsSheet() { return isSpriteSheet; }	

	int col;

private:
	std::vector<Texture*> textures;
	Texture* texture = nullptr;
	bool isSpriteSheet = true;
};