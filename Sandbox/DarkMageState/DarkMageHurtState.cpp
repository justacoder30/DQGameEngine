#include "DarkMageHurtState.h"
#include "../DarkMage.h"

void DarkMageHurtState::Enter()
{
    boss->Play(Hurt);

    boss->controller->velocity.x = 0;
}

void DarkMageHurtState::Update(float dt)
{
    if (boss->animationClip.IsDone())
    {
        if (boss->hp <= 0) boss->state->ChangeState(boss->deathState );
        else boss->state->ChangeState(boss->idleState );
    }
}