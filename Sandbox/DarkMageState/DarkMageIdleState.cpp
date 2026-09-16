#include "DarkMageIdleState.h"
#include "../DarkMage.h"
#include <cmath>
void DarkMageIdleState::Enter() {
    boss->Play(Idle);
    boss->controller->velocity.x = 0;
}
void DarkMageIdleState::Update(float) {
    if (!boss->HasTarget()) return;
    boss->FaceTarget();
    if (boss->ChooseAction()) return;
    const float distance = std::abs(boss->target->position.x-boss->position.x);
    if (distance < 160.f || distance > boss->attackRange)
        boss->state->ChangeState(boss->chaseState);
}
