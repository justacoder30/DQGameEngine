#include "DarkMageIdleState.h"
#include "../DarkMage.h"
#include "../Player.h"

void DarkMageIdleState::Enter()
{
    boss->Play(Idle);

    boss->controller->velocity.x = 0;
}

void DarkMageIdleState::Update(float dt)
{
    if (!boss->target) return;

    float dist = abs(boss->target->position.x - boss->position.x );

    if (dist <= boss->detectRange && !boss->maxMinions)
    {
        boss->state->ChangeState( boss->attack1State);
    }
}