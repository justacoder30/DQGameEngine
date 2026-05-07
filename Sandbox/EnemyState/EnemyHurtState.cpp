#include "EnemyHurtState.h"
#include "../Skeleton.h"

void EnemyHurtState::Enter()
{
	enemy->Play(Hurt);
	//enemy->controller->velocity.x = 0;
}

void EnemyHurtState::Update(float dt)
{
    if (enemy->animationClip.IsDone())
    {
        enemy->state->ChangeState(enemy->idleState);
    }
}
