#include "DarkMageAttackState.h"
#include "../DarkMage.h"
#include <algorithm>
void DarkMageAttackState::Enter() {
    hasCast = false;
    boss->casting = true;
    boss->FaceTarget();
    boss->Play(Attack1);
    boss->controller->velocity.x = 0;
    boss->summonTimer = 9.f;
    boss->castsSinceSummon = 0;
}
void DarkMageAttackState::Update(float) {
    if (!hasCast && boss->animationClip.animation.CurrentFrame >= 9) {
        hasCast = true; boss->SpawnMinion();
    }
    if (boss->animationClip.IsDone()) boss->FinishCast();
}
void DarkMageAttack2State::Enter() {
    shots = 0;
    doubleShot = boss->Enraged();
    boss->casting = true;
    boss->FaceTarget();
    boss->Play(Attack2);
    boss->controller->velocity.x = 0;
    ++boss->castsSinceSummon;
}
void DarkMageAttack2State::Update(float) {
    const int frame = boss->animationClip.animation.CurrentFrame;
    if (shots == 0 && frame >= 6) { boss->FireHead(); ++shots; }
    if (doubleShot && shots == 1 && frame >= 10) { boss->FireHead(); ++shots; }
    if (boss->animationClip.IsDone()) boss->FinishCast();
}
void DarkMageHealState::Enter() {
    healed = false;
    boss->casting = boss->healing = true;
    --boss->healsRemaining; // Interrupted attempts still cost a charge.
    boss->healTimer = 14.f;
    boss->Play(Heal);
    boss->controller->velocity.x = 0;
}
void DarkMageHealState::Update(float) {
    if (!healed && boss->animationClip.animation.CurrentFrame >= 10) {
        healed = true;
        boss->hp = std::min(boss->MaxHP,boss->hp+50);
        boss->healthbar->SetHealth(boss->hp,boss->MaxHP);
    }
    if (boss->animationClip.IsDone()) boss->FinishCast();
}
void DarkMageHealState::Exit() { boss->healing = false; }
