#pragma once
#include "PositionComponent.h"
#include "Core/Vector.h"
#include <glm/glm.hpp>

class CameraComponent : public Component
{
public:
    CameraComponent(float width, float height);

    void SetPosition(const Vector& pos) { m_Position = pos; }
    void SetZoom(float zoom) { m_Zoom = zoom; }
    void Follow(PositionComponent* target, float smooth = 5.0f);
    void SetBackdrop(PositionComponent* backdrop);

    glm::mat4 GetViewProjection() const { return m_ViewProjection; }

protected:
    void OnUpdate(float dt) override;
    void OnDraw() override;

private:
    void RecalculateMatrix();

    Vector m_Position;
    PositionComponent* m_Target = nullptr;
    PositionComponent* m_Backdrop = nullptr;

    float m_Width, m_Height;
    float m_Zoom = 1.0f;
    float m_SmoothSpeed = 5.0f;

    glm::mat4 m_ViewProjection;
};

