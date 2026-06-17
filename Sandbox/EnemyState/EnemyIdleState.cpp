#include "EnemyIdleState.h"
#include "../Skeleton.h"
#include "../Player.h"
#include <iostream>

void EnemyIdleState::Enter()
{
    enemy->Play(Idle);
    enemy->controller->velocity.x = 0;

    idleTime = 1.0f + (rand() / (float)RAND_MAX) * 2.0f;
}

void EnemyIdleState::Update(float dt)
{
    enemy->Play(Idle);
    if (enemy->target && enemy->onGround)
    {
        enemy->state->ChangeState(enemy->chaseState);
    }

    idleTime -= dt;

    if (idleTime <= 0.0f)
    {
        enemy->state->ChangeState(enemy->walkState);
    }
}