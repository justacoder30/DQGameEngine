#pragma once
#include "DQEngine.h"
#include "PlayerState/IdleState.h"
#include "PlayerState/RunState.h"
#include "PlayerState/JumpState.h"
#include "PlayerState/FallState.h"
#include "PlayerState/Attackstate.h"

class IdleState;

enum State
{
	Idle,
	Walk,
	Run,
	Jump,
	Fall,
	Attack1,
	Attack2,
	Attack3,
};

class Player : public Animation2DComponent, public CollisionCallbacks
{
public:
	Player();	

	void OnUpdate(float dt) override;
	void OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void SetSpawnPoint(const Vector& spawnPoint);	

	bool onGround = false;
	float jumpTime = 0.5;
	float jumpHeight = 100;
	float speed = 200;

	RectangleComponent* atkBox;
	RectangleComponent* hitbox;
	CharacterController* controller;
	StateMachineComponent* state;

	IdleState* idleState;
	RunState* runState;
	JumpState* jumpState;
	FallState* fallState;
	AttackState* attackState;

	float attackBuffer = 0;

private:

	std::vector<std::string> CreateStringAnimate(const std::string& f, int cout);
};

