#include "EnemyWalkState.h"
#include "../Skeleton.h"

void EnemyWalkState::Enter()
{
    enemy->Play(Run);
	enemy->controller->velocity.x = enemy->speed;

	walkTime = 2.0f + (rand() / (float)RAND_MAX) * 4.0f;

	enemy->controller->velocity.x = enemy->flip ? -enemy->speed : enemy->speed;
}

void EnemyWalkState::Update(float dt)
{
	if (enemy->target && enemy->onGround)
	{
		enemy->state->ChangeState(enemy->chaseState);
	}

	if (enemy->canTurn) {
		enemy->controller->velocity.x *= -1;
		enemy->HorizontalFlip();
		enemy->canTurn = false;
		enemy->flip = !enemy->flip;
	}

	walkTime -= dt;

	if (walkTime <= 0.0f)
	{
		enemy->state->ChangeState(enemy->idleState);
	}

}