#include "HurtState.h"
#include "../Player.h"

void HurtState::Enter()
{
    player->Play(Hurt);
	player->controller->velocity.x = 0;
	player->controller->velocity.y = 0;
}

void HurtState::Update(float dt)
{
    player->controller->velocity.x = 0;
    if (!player->onGround)
    {
        player->state->ChangeState(player->fallState);
        return;
    }

    if(player->animationClip.IsDone())
    {
        player->state->ChangeState(player->idleState);
        return;
	}
}
