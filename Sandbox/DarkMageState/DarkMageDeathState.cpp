#include "DarkMageDeathState.h"
#include "../DarkMage.h"

void DarkMageDeathState::Enter()
{
    boss->dead = true;

    boss->Play(Death);

    boss->controller->velocity.x = 0;

    boss->hitbox->active = false;
}

void DarkMageDeathState::Update(float dt)
{
    if (boss->animationClip.IsDone())
    {
        boss->RemoveFromParent();
    }
}