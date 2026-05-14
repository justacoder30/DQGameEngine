#pragma once
#include "PlayerState.h"
class AirAttackState : public PlayerState
{
public:
    using PlayerState::PlayerState;

    void Enter() override;
    void Update(float dt) override;
    void Exit() override;

private:
    int comboStep = 0;
    float timer = 0;
    bool hasHit = false;
    bool nextAttackQueued = false;

    void NextAttack();

};

