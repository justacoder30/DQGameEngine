#pragma once
#include "DQEngine.h"
#include "Player.h"
#include "EnumState.h"
#include "EnemyState/EnemyIdleState.h"
#include "EnemyState/EnemyChaseState.h"
#include "EnemyState/EnemyAttackState.h"
#include "EnemyState/EnemyHurtState.h"
#include "EnemyState/EnemyDeathState.h"
#include "EnemyState/EnemyWalkState.h"

class EnemyIdleState;
class EnemyChaseState;
class EnemyAttackState;
class EnemyHurtState;
class EnemyDeathState;

class Skeleton : public Animation2DComponent, public CollisionCallbacks
{
public:
	Skeleton(const Vector& pos);

	void OnLoad() override;	
	void OnUpdate(float dt) override;
	void OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;
	void OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other) override;

	RectangleComponent* sensor_player;
	RectangleComponent* sensor_ground;
	RectangleComponent* sensor_wall;
	RectangleComponent* sensor_edge;
	RectangleComponent* atkBox;
	RectangleComponent* hitbox;
	CharacterController* controller;

	StateMachineComponent* state;

	EnemyIdleState* idleState;
	EnemyChaseState* chaseState;
	EnemyAttackState* attackState;
	EnemyHurtState* hurtState;
	EnemyDeathState* deathState;
	EnemyWalkState* walkState;

	Player* target = nullptr;

	float attackRange = 40.f;
	float detectRange = 200.f;

	float attackCooldown = 1.0f;
	float attackTimer = 0;


	bool onGround = false;
	float speed = 80.f;	
	bool flip = false;	

	const int MaxHP = 100;	
	int hp = MaxHP;
	float atkDamage = 15;

	bool canTurn = false;

};

