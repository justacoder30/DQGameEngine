#include "EnemyDeathState.h"
#include "../Skeleton.h"

void EnemyDeathState::Enter()
{
    enemy->Play(Death);

    enemy->controller->velocity.x = 0;

    //enemy->hitbox->active = false;
}

void EnemyDeathState::Update(float dt)
{
    if (enemy->animationClip.IsDone())
    {
        enemy->RemoveFromParent();
    }
}