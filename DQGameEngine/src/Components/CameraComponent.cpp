#include "pch.h"
#include "CameraComponent.h"
#include "Renderer/Renderer2D.h"

CameraComponent::CameraComponent(float width, float height)
    : m_Width(width), m_Height(height)
{
    Renderer2D::SetViewport(width, height);
    RecalculateMatrix();
}

void CameraComponent::Follow(PositionComponent* target, float smooth)
{
    m_Target = target;
    m_SmoothSpeed = smooth;
}

Rect CameraComponent::GetViewBounds()
{
    float w = m_Width / m_Zoom;
    float h = m_Height / m_Zoom;

    return Rect(
        m_Position.x - w * 0.5f,
        m_Position.y - h * 0.5f,
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

void CameraComponent::OnUpdate(float dt)
{
    if (m_Target)
    {
        auto targetPos = m_Target->GetWorldPosition();
        m_Position.x += (targetPos.x - m_Position.x) * m_SmoothSpeed * dt;
        m_Position.y += (targetPos.y - m_Position.y) * m_SmoothSpeed * dt;
    }

    RecalculateMatrix();
}

void CameraComponent::OnAttach()
{
	Renderer2D::SetCamera(this);
}

void CameraComponent::RecalculateMatrix()
{
    m_HalfSize.x = (m_Width * 0.5f) / m_Zoom;
    m_HalfSize.y = (m_Height * 0.5f) / m_Zoom;

    glm::mat4 proj = glm::ortho(
        -m_HalfSize.x, m_HalfSize.x,
        m_HalfSize.y, -m_HalfSize.y,
        -1.0f, 1.0f
    );

    glm::mat4 view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-m_Position.x, -m_Position.y, 0.0f)
    );

    m_BackdropMatrix = proj;
    m_ViewProjection = m_BackdropMatrix * view;
}