#include "DarkMageChaseState.h"
#include "../DarkMage.h"
#include "../Player.h"

void DarkMageChaseState::Enter()
{
    boss->Play(Run);
}

void DarkMageChaseState::Update(float dt)
{
    if (!boss->target) return;

    float dx = boss->target->position.x - boss->position.x;

    float dist = abs(dx);

    //if (dist <= boss->attackRange)
    //{
    //    //boss->state->ChangeState( boss->attackState);

    //    return;
    //}

    float dir = dx > 0 ? 1.f : -1.f;

    boss->controller->velocity.x = dir * boss->speed;

    if (dir > 0 && boss->direction != Right)
    {
        boss->HorizontalFlip();
		boss->direction = Right;
    }
    else if (dir < 0 && boss->direction == Right)
    {
        boss->HorizontalFlip();
		boss->direction = Left;
    }
}