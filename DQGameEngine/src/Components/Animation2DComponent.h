#pragma once

#include <unordered_map>
#include "PositionComponent.h"
#include "AnimationClip.h"
#include "Core/Rect.h"
#include "Core/Color.h"

class Animation2DComponent : public PositionComponent
{
public:

    Animation2DComponent();
    void AddAnimation(int id, const Animation& animation);
    void Play(int id);
    void Stop();
    bool IsPlaying(int id);
    Rect GetSrcRect();
    void SetSize(float w, float h);
    void SetColor(const Color& color) { m_Color = color; }

protected:

    void OnUpdate(float dt) override;
    void OnDraw() override;

    int currentAnimation = -1;
    AnimationClip animationClip;
    std::unordered_map<int, Animation> animations;

private:
    Color m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    bool playing = false;
};