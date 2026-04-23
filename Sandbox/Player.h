#pragma once
#include "DQEngine.h"

class Player : public Animation2DComponent, public CollisionCallbacks
{
public:
	Player();	

	void OnUpdate(float dt) override;
	void OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void SetSpawnPoint(const Vector& spawnPoint);	

	float jumpTime = 0.5;
	float jumpHeight = 100;
	float speed = 200;
	RectangleComponent* hitbox;
	CharacterController* controller;

private:

	std::vector<std::string> CreateStringAnimate(const std::string& f, int cout);

	bool onGround = false;
	RigidbodyComponent* rb;
};

