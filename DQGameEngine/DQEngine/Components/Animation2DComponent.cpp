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
	if (currentAnimation == id) return;
    
    currentAnimation = id;
    animationClip.Play(animations[id]);
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
    animationClip.Update(dt);
    src = animationClip.getRect();
	PositionComponent::OnUpdate(dt);    
}

void Animation2DComponent::OnDraw()
{
    if (!m_Visible)
        return;

    if (animationClip.animation.GetCurrentTexture() == nullptr) return;
    
    if (layer == RenderLayer::World && !Renderer2D::GetCamera()->CanSee(dst))
        return;

    Renderer2D::Submit({
        CommandType::Sprite,
        animationClip.animation.GetCurrentTexture(),
        src,
        dst,
        flip,
        angle,
        anchor,
        layer,
        m_Color
    });

    //Renderer2D::DrawRectOutline(dst, 1.f);
}