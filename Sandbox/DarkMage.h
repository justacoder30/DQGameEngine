#pragma once
#include "DQEngine.h"
#include "EnumState.h"
#include "Player.h"
#include "Healthbar.h"

#include "DarkMageState/DarkMageIdleState.h"
#include "DarkMageState/DarkMageChaseState.h"
#include "DarkMageState/DarkMageHurtState.h"
#include "DarkMageState/DarkMageDeathState.h"
#include "DarkMageState/DarkMageAttackState.h"


class DarkMageIdleState;
class DarkMageChaseState;
class DarkMageAttackState;
class DarkMageHurtState;
class DarkMageDeathState;

class DarkMage : public Animation2DComponent, public CollisionCallbacks
{
public:
	DarkMage(const Vector& pos);
	void OnLoad() override;
	void OnUpdate(float dt) override;
	void OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	//void OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;

	void SpawnMinion();

	RectangleComponent* sensor_player;
	RectangleComponent* hitbox;
	CharacterController* controller;
	Player* target = nullptr;
	StateMachineComponent* state;

	DarkMageIdleState* idleState;
	DarkMageChaseState* chaseState;
	DarkMageAttackState* attack1State;
	DarkMageHurtState* hurtState;
	DarkMageDeathState* deathState;

	Healthbar* healthbar;

	float speed = 120.f;

	const int MaxHP = 200;
	int hp = MaxHP;
	float scale = 2.0f;
	bool dead = false;

	bool facingRight = true;
	bool maxMinions = false;

	float detectRange = 300.f;
	float attackRange = 120.f;

	float attackCooldown = 1.5f;
	float attackTimer = 0;

	Direction direction = Right;
};

