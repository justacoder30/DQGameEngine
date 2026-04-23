#include "AttackState.h"
#include "../Player.h"

struct AttackData
{
    float duration;
    float hitTime;
    float comboWindow;
};

static AttackData attacks[] =
{
    {0.4f, 0.2f, 0.3f}, // Attack1
    {0.45f, 0.25f, 0.35f}, // Attack2
    {0.5f, 0.3f, 0.4f}, // Attack3
};

AttackState::AttackState(Player* p) : PlayerState(p) {}

void AttackState::Enter()
{
    timer = 0;
    hasHit = false;

    player->Play(Attack1 + comboStep);
}

void AttackState::Update(float dt)
{
    timer += dt;

    auto& atk = attacks[comboStep];

    if (!hasHit && timer >= atk.hitTime)
    {
        player->atkBox->active = true;
        hasHit = true;
    }

    if (timer >= atk.comboWindow && player->attackBuffer > 0)
    {
        player->attackBuffer = 0;
        NextAttack();
        return;
    }

    if (timer >= atk.duration)
    {
        player->state->ChangeState(player->idleState);
    }
}

void AttackState::Exit()
{
    comboStep = 0;
}

void AttackState::NextAttack()
{
    comboStep++;

    if (comboStep > 2)
    {
        player->state->ChangeState(player->idleState);
        return;
    }

    timer = 0;
    hasHit = false;

    player->Play(Attack1 + comboStep);
}