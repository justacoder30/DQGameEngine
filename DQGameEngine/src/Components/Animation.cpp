#include "pch.h"
#include "Animation.h"
#include "Core/TextureManager.h"

Animation::Animation()
{
}

Animation::Animation(const char* f_p, int frame_count, float frame_speed, bool Loop, const int& col, const int& FrameHeightCount, const Vector& Framesize)
{
	this->col = col;
	texture = TextureManager::Load(f_p);
	FrameCount = frame_count;
	FrameSpeed = frame_speed;
	loop = Loop;
	CurrentFrame = 0;

	if (Framesize.x != 0 && Framesize.y != 0)
	{
		FrameWidth = Framesize.x;
		FrameHeight = Framesize.y;
	}
	else
	{
		FrameWidth = texture->GetWidth() / FrameCount;
		FrameHeight = texture->GetHeight() / FrameHeightCount;
	}

    isSpriteSheet = true;
}

Animation::Animation(const std::vector<std::string>& paths, float frame_speed, bool Loop)
{
    for (auto p : paths)
    {
        textures.push_back(TextureManager::Load(p));
    }

    FrameCount = paths.size();
    FrameSpeed = frame_speed;
    loop = Loop;
    CurrentFrame = 0;

    isSpriteSheet = false;

    FrameWidth = textures[0]->GetWidth();
    FrameHeight = textures[0]->GetHeight();
}

Texture* Animation::GetCurrentTexture()
{
    if (isSpriteSheet) return texture;
    return textures[CurrentFrame];
}

int Animation::LastFrame()
{
	return FrameCount - 1;
}
