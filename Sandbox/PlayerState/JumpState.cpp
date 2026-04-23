#include "JumpState.h"
#include "../Player.h"

void JumpState::Enter()
{
    player->Play(Jump);
}

void JumpState::Update(float dt)
{
    if (player->controller->velocity.y > 0)
    {
        player->state->ChangeState(player->fallState);
        return;
    }
}
