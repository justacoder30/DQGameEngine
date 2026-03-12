#pragma once

#include <unordered_map>
#include "Component.h"
#include "AnimationClip.h"
#include "Core/Rect.h"

class Animation2DComponent : public Component
{
public:

    Animation2DComponent();

    void AddAnimation(int id, const Animation& animation);

    void Play(int id);

    void Stop();

    bool IsPlaying(int id);

    Rect GetSrcRect();

    void SetPosition(float x, float y);
    void SetSize(float w, float h);

protected:
    int currentAnimation = -1;
    AnimationClip animationClip;

    void OnUpdate(float dt) override;
    void OnDraw() override;

    Rect src;
    Rect dst;

private:

    std::unordered_map<int, Animation> animations;
    bool playing = false;
};