#include "DarkMageHurtState.h"
#include "../DarkMage.h"
void DarkMageHurtState::Enter() {
    boss->casting = boss->healing = false;
    boss->Play(Hurt);
    boss->animationClip.ResetFrame();
    boss->controller->velocity.x = 0;
}
void DarkMageHurtState::Update(float) {
    if (boss->animationClip.IsDone()) {
        boss->attackTimer = 0.55f;
        boss->state->ChangeState(boss->idleState);
    }
}
