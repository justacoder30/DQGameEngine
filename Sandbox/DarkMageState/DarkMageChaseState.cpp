#include "DarkMageChaseState.h"
#include "../DarkMage.h"
#include <cmath>
void DarkMageChaseState::Enter() { boss->Play(Run); }
void DarkMageChaseState::Update(float) {
    if (!boss->HasTarget()) { boss->state->ChangeState(boss->idleState); return; }
    if (boss->ChooseAction()) return;
    boss->FaceTarget();
    const float dx = boss->target->position.x-boss->position.x;
    const float distance = std::abs(dx);
    float direction = dx > 0 ? 1.f : -1.f;
    if (distance < 190.f) direction = -direction;
    else if (distance <= boss->attackRange-25.f) {
        boss->state->ChangeState(boss->idleState); return;
    }
    boss->controller->velocity.x = boss->CanMove(direction) ?
        direction*boss->speed*(boss->Enraged() ? 1.2f : 1.f) : 0.f;
}
