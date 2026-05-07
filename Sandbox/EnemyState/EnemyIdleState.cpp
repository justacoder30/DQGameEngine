#include "EnemyIdleState.h"
#include "../Skeleton.h"
#include "../Player.h"

void EnemyIdleState::Enter()
{
    enemy->Play(Idle);
    enemy->controller->velocity.x = 0;
}

void EnemyIdleState::Update(float dt)
{
    if (enemy->target && enemy->onGround)
    {
        float dist = abs(enemy->position.x - enemy->target->position.x);

        if (dist <= enemy->detectRange)
        {
            enemy->state->ChangeState(enemy->chaseState);
        }
    }
}