#pragma once

#include <unordered_map>
#include "PositionComponent.h"
#include "AnimationClip.h"
#include "Core/Rect.h"

class Animation2DComponent : public PositionComponent
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

    void OnUpdate(float dt) override;
    void OnDraw() override;

    int currentAnimation = -1;
    AnimationClip animationClip;

private:

    std::unordered_map<int, Animation> animations;
    bool playing = false;
    Rect src;
    Rect dst;
};