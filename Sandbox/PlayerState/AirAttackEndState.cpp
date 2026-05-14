#include "AirAttackEndState.h"
#include "../Player.h"

void AirAttackEndState::Enter()
{
	player->atkEndBox->active = true;
	player->Play(AirAttackEnd);
}

void AirAttackEndState::Update(float dt)
{
	player->controller->velocity.x = 0;
	
	if (player->animationClip.IsDone())
	{
		player->state->ChangeState(player->idleState);
	}
}
