#include "EnemyAttackState.h"
#include "../Skeleton.h"

void EnemyAttackState::Enter()
{
    enemy->Play(Attack1);

    enemy->controller->velocity.x = 0;
	hasHit = false;
    enemy->attackTimer = 0;
}

void EnemyAttackState::Update(float dt)
{
    enemy->attackTimer += dt;

    if (!hasHit && enemy->attackTimer >= 0.6f)
    {
        hasHit = true;
        enemy->atkBox->active = true;
    }

    if (enemy->animationClip.IsDone())
    {
        enemy->atkBox->active = false;
        enemy->state->ChangeState(enemy->chaseState);
    }
}
