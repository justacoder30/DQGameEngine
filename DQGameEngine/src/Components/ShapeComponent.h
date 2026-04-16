#pragma once
#include "PositionComponent.h"
#include "Core/CollisionCallbacks.h"

enum class Layer : uint32_t
{
	None = 0,
	Player = 1 << 0,
	Enemy = 1 << 1,
	Ground = 1 << 2,
	Attack = 1 << 3,
	Sensor = 1 << 4,
	Item = 1 << 5,
};

inline uint32_t ToMask(Layer layer)
{
	return static_cast<uint32_t>(layer);
}

inline bool HasLayer(uint32_t mask, Layer layer)
{
	return (mask & ToMask(layer)) != 0;
}

enum ShapeType
{
	Rectangle,
	Circle,
	Polygon
};	

class ShapeComponent : public PositionComponent
{
public:

	ShapeComponent(){}
	ShapeComponent(const Vector& position, const Vector& size) : PositionComponent(position, size) {}
	
	virtual bool CheckCollide(ShapeComponent* other) { return false; }
	
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;

	uint32_t mask = 0;
	Layer layer = Layer::None;

	bool isTrigger = false;
	bool hasPhysics = true;
	bool active = true;
	bool isColliding = false;

	ShapeType shapeType = ShapeType::Rectangle;
	CollisionCallbacks* callback = nullptr;
};

