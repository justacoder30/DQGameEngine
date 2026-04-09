#pragma once
#include "DQEngine.h"

class Player : public Animation2DComponent, public CollisionCallbacks
{
public:
	Player();	

	void OnUpdate(float dt) override;
	//void OnCollision(ShapeComponent* shape, Component* other) override;
	void OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void SetSpawnPoint(const Vector& spawnPoint);	

	float jumpTime = 0.5;
	float jumpHeight = 100;
	float graviry = 500;
	float jump = 320;
	float speed = 200;
	Vector velocity;

private:

	bool onGround = false;
	RigidbodyComponent* rb;
};

