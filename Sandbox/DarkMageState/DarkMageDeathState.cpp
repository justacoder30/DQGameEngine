#include "DarkMageDeathState.h"
#include "../DarkMage.h"
void DarkMageDeathState::Enter() {
    boss->dead = true;
    boss->ClearAttacks();
    boss->Play(Death);
    boss->controller->velocity = Vector(0,0);
    boss->hitbox->active = false;
    boss->sensor_player->active = false;
    boss->healthbar->SetHealth(0,boss->MaxHP);
}
void DarkMageDeathState::Update(float) {
    if (boss->animationClip.IsDone()) boss->RemoveFromParent();
}
