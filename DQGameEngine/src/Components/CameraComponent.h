#pragma once
#include "PositionComponent.h"
#include "Core/Vector.h"
#include "Core/Rect.h"
#include <glm/glm.hpp>

class CameraComponent : public Component
{
public:
    CameraComponent(float width, float height);

    void SetPosition(const Vector& pos) { m_Position = pos; }
    void SetZoom(float zoom) { m_Zoom = zoom; }
    void Follow(PositionComponent* target, float smooth = 5.0f);
    void SetBounds(const Rect& bounds);
    const Rect& GetViewBounds() const;
	bool CanSee(PositionComponent* component);
	bool CanSee(const Rect& bounds);
    void SetBackdropMatrix(const glm::mat4& backdropMatrix) { m_BackdropMatrix = backdropMatrix; }

    glm::mat4 GetViewProjection() const { return m_ViewProjection; }
    glm::mat4 GetBackdropMatrix() const { return m_BackdropMatrix; }

	PositionComponent* GetTarget() const { return m_Target; }


protected:
    void OnUpdate(float dt) override;
	void OnAttach() override;

private:
    void RecalculateMatrix();

    Vector m_Position;
	Vector m_HalfSize;
    PositionComponent* m_Target = nullptr;

    float m_Width, m_Height;
    float m_Zoom = 1.0f;
    float m_SmoothSpeed = 5.0f;
    bool m_UseBounds = false;
    Rect m_Bounds;

    glm::mat4 m_ViewProjection;
    glm::mat4 m_BackdropMatrix;
};

