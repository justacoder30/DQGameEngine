#include "RollState.h"
#include <iostream>
#include "../Player.h"

RollState::RollState(Player* p) : PlayerState(p)
{
}

void RollState::Enter()
{
    timer = 0;

    player->Play(Roll);

	player->rollHitbox->active = true;
	player->hitbox->active = false;
	player->controller->hitbox = player->rollHitbox;

	player->rollHitbox->active = true;
	player->hitbox->active = false; 

	player->groundBox->active = false;
	player->rollGroundBox->active = true;

    player->invincible = true;
    dir = player->direction == Right ? 1 : -1;
    player->controller->velocity.x = dir * rollSpeed;
    player->controller->velocity.y = 0;

	auto rollHitbox_bounds = player->rollHitbox->GetBounds();
	auto hitbox_bounds = player->hitbox->GetBounds();

	player->position.y = player->position.y + (hitbox_bounds.h - rollHitbox_bounds.h);
}

void RollState::Update(float dt)
{
    timer += dt;

    player->controller->velocity.x = dir * rollSpeed;

    if (timer >= duration)
    {
        if (player->controller->velocity.x != 0) 
            player->state->ChangeState(player->runState);
        else 
            player->state->ChangeState(player->idleState);
    }
}

void RollState::Exit()
{
    player->invincible = false;
    player->rollHitbox->active = false;
    player->hitbox->active = true;
    player->controller->hitbox = player->hitbox;

    player->rollHitbox->active = false;
    player->hitbox->active = true;

    auto rollHitbox_bounds = player->rollHitbox->GetBounds();
    auto hitbox_bounds = player->hitbox->GetBounds();

    player->groundBox->active = true;
    player->rollGroundBox->active = false;

    player->position.y = player->position.y - (hitbox_bounds.h - rollHitbox_bounds.h);
}