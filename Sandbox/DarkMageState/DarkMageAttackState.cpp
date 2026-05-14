#include "DarkMageAttackState.h"
#include "../DarkMage.h"

void DarkMageAttackState::Enter()
{
    timer = 0;

    hasCast = false;

    boss->Play(Attack1);

    boss->controller->velocity.x = 0;
}

void DarkMageAttackState::Update(float dt)
{
    timer += dt;

    if (!hasCast && boss->animationClip.animation.CurrentFrame >= 10)
    {
        hasCast = true;
        boss->SpawnMinion();
    }

    if (boss->animationClip.IsDone())
    {
        boss->state->ChangeState(boss->idleState );
    }
}