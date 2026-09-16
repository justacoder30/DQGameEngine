#pragma once
#include "Engine.h"
#include "EnumState.h"
#include "Player.h"
#include "Healthbar.h"
#include "DarkMageState/DarkMageIdleState.h"
#include "DarkMageState/DarkMageChaseState.h"
#include "DarkMageState/DarkMageHurtState.h"
#include "DarkMageState/DarkMageDeathState.h"
#include "DarkMageState/DarkMageAttackState.h"
#include "Head.h"

class DarkMage : public Animation2DComponent, public CollisionCallbacks
{
public:
    explicit DarkMage(const Vector& pos);
    ~DarkMage() override;
    void OnLoad() override;
    void OnUpdate(float dt) override;
    void OnCollisionStart(ShapeComponent*, ShapeComponent*, Component*) override;
    void OnCollision(ShapeComponent*, ShapeComponent*, Component*) override;
    void SpawnMinion();
    void FireHead();
    void FinishCast();
    void FaceTarget();
    bool ChooseAction();
    bool CanMove(float direction) const;
    bool Enraged() const { return hp <= MaxHP / 2; }
    bool HasTarget() const { return target && target->hp > 0; }
    void ClearAttacks();
    size_t ActiveHeads() const;

    RectangleComponent* sensor_player = nullptr;
    RectangleComponent* hitbox = nullptr;
    CharacterController* controller = nullptr;
    Player* target = nullptr;
    StateMachineComponent* state = nullptr;
    DarkMageIdleState* idleState = nullptr;
    DarkMageChaseState* chaseState = nullptr;
    DarkMageAttackState* attack1State = nullptr;
    DarkMageAttack2State* attack2State = nullptr;
    DarkMageHealState* healState = nullptr;
    DarkMageHurtState* hurtState = nullptr;
    DarkMageDeathState* deathState = nullptr;
    Healthbar* healthbar = nullptr;

    const int MaxHP = 400;
    int hp = MaxHP;
    float scale = 2.0f;
    float speed = 105.f;
    float detectRange = 540.f;
    float attackRange = 330.f;
    float attackCooldown = 1.8f;
    float attackTimer = 1.0f;
    float summonTimer = 4.f;
    float healTimer = 0.f;
    float damageGrace = 0.f;
    float staggerTimer = 0.f;
    bool dead = false;
    bool casting = false;
    bool healing = false;
    bool facingRight = true;
    bool maxMinions = false;
    Direction direction = Right;
    int healsRemaining = 2;
    int summonsRemaining = 4;
    int castsSinceSummon = 0;

private:
    std::shared_ptr<HeadVolley> headVolley = Shared<HeadVolley>();
    Vector home;
};
