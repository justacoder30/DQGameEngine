#include "FallState.h"
#include "../Player.h"

void FallState::Enter()
{
    player->Play(Fall);
}

void FallState::Update(float dt)
{
    if (player->onGround)
    {
        player->state->ChangeState(player->idleState);
        return;
    }
}
