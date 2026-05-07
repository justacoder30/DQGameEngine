#include "DeathState.h"
#include "../Player.h"

void DeathState::Enter()
{
    player->Play(Death);
    player->controller->velocity.x = 0;
    player->controller->velocity.y = 0;
}

void DeathState::Update(float dt)
{
    player->controller->velocity.x = 0;
    if (player->animationClip.IsDone())
    {
        player->ReSpawn();
    }
}
