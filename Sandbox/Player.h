#pragma once
#include "DQEngine.h"
#include "EnumState.h"
#include "PlayerState/IdleState.h"
#include "PlayerState/RunState.h"
#include "PlayerState/JumpState.h"
#include "PlayerState/FallState.h"
#include "PlayerState/Attackstate.h"
#include "PlayerState/AirAttackstate.h"
#include "PlayerState/AirAttackEndstate.h"
#include "PlayerState/HurtState.h"
#include "PlayerState/DeathState.h"
#include "PlayerState/RollState.h"
#include "Healthbar.h"

class IdleState;

struct AttackData
{
	float duration;
	float hitTime;
	float comboWindow;
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
	void ReSpawn();

	bool onGround = false;
	float jumpTime = 0.5;
	float jumpHeight = 100;
	float speed = 200;

	AttackData attacks[3] = {
		{0.3f, 0.15f, 0.15f}, // Attack1
		{0.3f, 0.15f, 0.15f}, // Attack2
		{0.3f, 0.15f, 0.15f}, // Attack3
	};

	AttackData airAttacks[4] = {
		{0.25f, 0.1f, 0.15f}, // Attack1
		{0.25f, 0.1f, 0.15f}, // Attack2
		{0.25f, 0.1f, 0.15f}, // Attack3
		{0.4f, 0.1f, 0.15f}, // AttackEnd
	};


	RectangleComponent* atkBox;
	RectangleComponent* atkEndBox;
	RectangleComponent* hitbox;
	RectangleComponent* rollGroundBox;
	RectangleComponent* groundBox;
	RectangleComponent* rollHitbox;
	CharacterController* controller;
	StateMachineComponent* state;

	IdleState* idleState;
	RunState* runState;
	JumpState* jumpState;
	FallState* fallState;
	AttackState* attackState;
	AirAttackState* airAttackState;
	AirAttackEndState* airAttackEndkState;
	HurtState* hurtState;
	DeathState* deathState;
	RollState* rollState;
	//DeathState* DeathState;

	Healthbar* healthbar;

	const int MaxHP = 100;
	int hp = MaxHP;
	float atkDamage = 25;
	float attackBuffer = 0;
	bool invincible = false;
	Direction direction = Right;

private:
	Vector spawnPoint;
	std::vector<std::string> CreateStringAnimate(const std::string& f, int cout);
};

