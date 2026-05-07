#pragma once
#include "Component.h"
#include "Core/Vector.h"
#include "Core/Flip.h"
#include "Renderer/RenderCommand.h"

class PositionComponent : public Component
{
public:

	PositionComponent() : position(0), size(0) {}
	PositionComponent(const Vector& position, const Vector& size = 0) : position(position), size(size) {}	

	void OnUpdate(float dt) override;
	void OnDraw() override;

	const Vector& GetWorldPosition();
	void SetPosition(float x, float y);
	void SetAnchor(float x, float y);
	void HorizontalFlip();
	void VerticalFlip();
	
	bool m_Visible = true;
	float angle = 0.0f;
	Vector position;
	Vector size;
	Vector anchor;
	Flip flip  = Flip::None;
	RenderLayer layer = RenderLayer::World;

	Rect src;
	Rect dst;

protected:

};

