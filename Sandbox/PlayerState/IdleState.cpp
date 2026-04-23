#include "IdleState.h"
#include "../Player.h"

void IdleState::Enter()
{
    player->Play(Idle);
}

void IdleState::Update(float dt)
{
    if (!player->onGround && player->controller->velocity.y > 0)
    {
        player->state->ChangeState(player->fallState);
        return;
    }

    if (player->controller->velocity.y < 0)
    {
        player->state->ChangeState(player->jumpState);
        return;
    }

    if (player->controller->velocity.x != 0)
    {
        player->state->ChangeState(player->runState);
        return;
    }

    if (player->attackBuffer > 0)
    {
        player->state->ChangeState(player->attackState);
        return;
    }
}