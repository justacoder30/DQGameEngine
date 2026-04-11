#include "pch.h"
#include "Animation2DComponent.h"
#include "Renderer/Renderer2D.h"

Animation2DComponent::Animation2DComponent()
{
}

void Animation2DComponent::AddAnimation(int id, const Animation& animation)
{
    animations[id] = animation;
	if (size == Vector::Zero())
    {
        size.x = animation.FrameWidth;
        size.y = animation.FrameHeight;
    }
}

void Animation2DComponent::Play(int id)
{
    animationClip.Play(animations[id]);
    currentAnimation = id;
    playing = true;
}

void Animation2DComponent::Stop()
{
    animationClip.stop();
    playing = false;
}

bool Animation2DComponent::IsPlaying(int id)
{
    return currentAnimation == id;
}

Rect Animation2DComponent::GetSrcRect()
{
    return animationClip.getRect();
}

void Animation2DComponent::SetSize(float w, float h)
{
    size.x = w;
    size.y = h;
}

void Animation2DComponent::OnUpdate(float dt)
{
    animationClip.Play(animations[currentAnimation]);
    animationClip.Update(dt);
    src = animationClip.getRect();
	PositionComponent::OnUpdate(dt);    
}

void Animation2DComponent::OnDraw()
{
    if (animationClip.animation.texture == nullptr) return;
    
    if (layer == RenderLayer::World && !Renderer2D::GetCamera()->CanSee(dst))
        return;

    Renderer2D::Submit({
        CommandType::Sprite,
        animationClip.animation.texture,
        src,
        dst,
        flip,
        angle,
        anchor,
        //Vector(0, 0),
        layer
    });

    //Renderer2D::DrawRectOutline(dst, 1.f);
}