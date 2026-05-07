#pragma once
#include "DQEngine.h"

class Skeleton : public Animation2DComponent, public CollisionCallbacks
{
public:
	Skeleton(const Vector& pos);

	void OnLoad() override;	
	void OnUpdate(float dt) override;
	void OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;

	RectangleComponent* sensor_ground;
	RectangleComponent* sensor_wall;
	RectangleComponent* sensor_edge;
	RectangleComponent* hitbox;
	CharacterController* controller;


	bool onGround = true;
	float speed = 80.f;	
	bool flip = false;	

	const int MaxHP = 100;	
	int hp = MaxHP;
	float atkDamage = 15;

};

