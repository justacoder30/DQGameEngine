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

void CameraComponent::SetBackdrop(PositionComponent* backdrop)
{
	m_Backdrop = backdrop;
    m_Backdrop->Load();
}

void CameraComponent::OnUpdate(float dt)
{
    if (m_Target)
    {
        auto targetPos = m_Target->GetWoldPosition();
        m_Position.x += (targetPos.x - m_Position.x) * m_SmoothSpeed * dt;
        m_Position.y += (targetPos.y - m_Position.y) * m_SmoothSpeed * dt;
    }

    RecalculateMatrix();
    Renderer2D::SetCamera(m_ViewProjection);
}

void CameraComponent::OnDraw()
{
    if (!m_Backdrop) return;

    m_Backdrop->SetPosition(m_Position.x, m_Position.y);
    m_Backdrop->Draw();
}

void CameraComponent::RecalculateMatrix()
{
    float halfW = (m_Width * 0.5f) / m_Zoom;
    float halfH = (m_Height * 0.5f) / m_Zoom;

    glm::mat4 proj = glm::ortho(
        -halfW, halfW,
        halfH, -halfH,
        -1.0f, 1.0f
    );

    glm::mat4 view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-m_Position.x, -m_Position.y, 0.0f)
    );

    m_ViewProjection = proj * view;
}