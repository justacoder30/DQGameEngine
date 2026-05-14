#include "AirAttackState.h"
#include "../Player.h"

void AirAttackState::Enter()
{
    timer = 0;
    hasHit = false;
	nextAttackQueued = false;
    player->Play(AirAttack1 + comboStep);
}

void AirAttackState::Update(float dt)
{
    timer += dt;

    auto& atk = player->attacks[comboStep];

    //player->controller->velocity.y = 0;

    if (!hasHit && timer >= atk.hitTime)
    {
        player->atkBox->active = true;
        hasHit = true;
    }

    if (timer >= atk.comboWindow && player->attackBuffer > 0)
    {
        nextAttackQueued = true;
    }

	if (comboStep == 2 && player->onGround) {
		player->state->ChangeState(player->airAttackEndkState);
		return;
	}

    if (player->animationClip.IsDone())
    {
        if (nextAttackQueued == true) {
            player->attackBuffer = 0;
            NextAttack();
            return;
        }

        player->state->ChangeState(player->fallState);
    }
}

void AirAttackState::Exit()
{
    comboStep = 0;
    nextAttackQueued = false;
}

void AirAttackState::NextAttack()
{
    comboStep++;

    if (comboStep > 2)
    {
        player->state->ChangeState(player->fallState);
        return;
    }

    nextAttackQueued = false;
    timer = 0;
    hasHit = false;

    player->Play(AirAttack1 + comboStep);
}
