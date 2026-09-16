#include "pch.h"
#include "CameraComponent.h"
#include "Renderer/Renderer2D.h"
#include "Core/Utils.h"

CameraComponent::CameraComponent(float width, float height)
    : m_Width(width), m_Height(height)
{
    Vector windowSize = Renderer2D::GetWindowSize();

    float scale = std::min(windowSize.x / width, windowSize.y / height);

    float vpWidth = width * scale;
    float vpHeight = height * scale;

    float vpX = (windowSize.x - vpWidth) * 0.5f;
    float vpY = (windowSize.y - vpHeight) * 0.5f;

    glViewport((int)vpX, (int)vpY, (int)vpWidth, (int)vpHeight);

    glm::mat4 proj = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    RecalculateMatrix();
}

void CameraComponent::Follow(PositionComponent* target, float smooth)
{
    m_Target = target;
    m_SmoothSpeed = smooth;
}

void CameraComponent::SetBounds(const Rect& bounds)
{
    m_Bounds = bounds;
    m_UseBounds = true;
}

Rect CameraComponent::GetViewBounds() const
{
    float w = m_Width / m_Zoom;
    float h = m_Height / m_Zoom;

    return Rect(
        m_Position.x - w * m_Origin.x,
        m_Position.y - h * m_Origin.y,
        w,
        h
    );
}

bool CameraComponent::CanSee(PositionComponent* component)
{
    auto pos = component->GetWorldPosition();
	Rect compBounds(pos, component->size);  
    Rect view = GetViewBounds();

	return view.CheckCollide(compBounds);
}

bool CameraComponent::CanSee(const Rect& bounds)
{
    Rect view = GetViewBounds();

	return view.CheckCollide(bounds);
}

void CameraComponent::Shake(float strength, float duration)
{
    shakeStrength = strength;
    shakeDuration = duration;
    shakeTimer = duration;
}

void CameraComponent::OnUpdate(float dt)
{
    if (shakeTimer > 0)
    {
        shakeTimer -= dt;

        shakeOffset.x = RandomFloat(-shakeStrength, shakeStrength);

        shakeOffset.y = RandomFloat(-shakeStrength, shakeStrength);
    }
    else shakeOffset = Vector::Zero();

    if (m_Target)
    {

        m_Position.x += (m_Target->position.x - m_Position.x) * m_SmoothSpeed * dt;
        m_Position.y += (m_Target->position.y - m_Position.y) * m_SmoothSpeed * dt;
    }

    if (m_UseBounds)
    {
        float halfW = m_Width * 0.5f / m_Zoom;
        float halfH = m_Height * 0.5f / m_Zoom;

        float minX = m_Bounds.x + halfW;
        float maxX = m_Bounds.x + m_Bounds.w - halfW;

        float minY = m_Bounds.y + halfH;
        float maxY = m_Bounds.y + m_Bounds.h - halfH;

        m_Position.x = std::clamp(m_Position.x, minX, maxX);
        m_Position.y = std::clamp(m_Position.y, minY, maxY);
    }

    RecalculateMatrix();
}

void CameraComponent::OnAttach()
{
	Renderer2D::SetCamera(this);
}

void CameraComponent::RecalculateMatrix()
{
    glm::mat4 proj = glm::ortho(
        0.0f, m_Width / m_Zoom,
        m_Height / m_Zoom, 0.0f,
        -1.0f, 1.0f
    );

    float originX = m_Origin.x * m_Width;

    float originY = m_Origin.y * m_Height;

    glm::mat4 view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(
            -m_Position.x + originX + shakeOffset.x,
            -m_Position.y + originY + shakeOffset.y,
            0.0f
        )
    );

    m_BackdropMatrix = proj;
    m_ViewProjection = m_BackdropMatrix * view;
}