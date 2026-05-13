#include "HurtState.h"
#include "../Player.h"

void HurtState::Enter()
{
    player->Play(Hurt);

	player->controller->velocity.x = 0;
	player->controller->velocity.y = 0;

    Color color;
    color.r = 1.0f;
    color.g = 0.f;
    color.b = 0.f;
    color.a = 1.f;

    player->SetColor(color);
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

void HurtState::Exit()
{
    Color color;

	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;

    player->SetColor(color);
}
