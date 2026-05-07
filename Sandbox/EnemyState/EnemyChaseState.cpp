#include "EnemyChaseState.h"
#include "../Skeleton.h"
#include "../Player.h"

void EnemyChaseState::Enter()
{
	enemy->Play(Run);
}

void EnemyChaseState::Update(float dt)
{
    if (!enemy->target)
    {
        enemy->state->ChangeState(enemy->idleState);
        return;
    }

    float dist = abs(enemy->position.x - enemy->target->position.x);

    if (dist <= enemy->attackRange)
    {
        enemy->state->ChangeState(enemy->attackState);
        return;
    }

    float dir =
        (enemy->position.x < enemy->target->position.x)
        ? 1.0f : -1.0f;

    enemy->controller->velocity.x = dir * enemy->speed;

    if (dir > 0 && enemy->flip)
    {
        enemy->HorizontalFlip();
        enemy->flip = false;
    }
    else if (dir < 0 && !enemy->flip)
    {
        enemy->HorizontalFlip();
        enemy->flip = true;
    }
}
