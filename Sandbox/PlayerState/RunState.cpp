#pragma once
#include "../Player.h"

void RunState::Enter()
{
    player->Play(Run);
}

void RunState::Update(float dt)
{
    if (!player->onGround)
    {
        player->state->ChangeState(player->fallState);
        return;
    }

    if (player->controller->velocity.y < 0)
    {
        player->state->ChangeState(player->jumpState);
        return;
    }

    if (player->controller->velocity.x == 0)
    {
        player->state->ChangeState(player->idleState);
        return;
    }
}
