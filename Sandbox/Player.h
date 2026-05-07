#pragma once
#include "DQEngine.h"
#include "PlayerState/IdleState.h"
#include "PlayerState/RunState.h"
#include "PlayerState/JumpState.h"
#include "PlayerState/FallState.h"
#include "PlayerState/Attackstate.h"
#include "Healthbar.h"

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

	void OnLoad() override;
	void OnUpdate(float dt) override;
	void OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void SetSpawnPoint(const Vector& spawnPoint);	

	bool onGround = false;
	float jumpTime = 0.5;
	float jumpHeight = 100;
	float speed = 200;
	//float speed = 20000;
	//float speed = 100000;

	RectangleComponent* atkBox;
	RectangleComponent* hitbox;
	CharacterController* controller;
	StateMachineComponent* state;

	IdleState* idleState;
	RunState* runState;
	JumpState* jumpState;
	FallState* fallState;
	AttackState* attackState;

	Healthbar* healthbar;

	float atkDamage = 25;
	float attackBuffer = 0;

private:

	std::vector<std::string> CreateStringAnimate(const std::string& f, int cout);
};

