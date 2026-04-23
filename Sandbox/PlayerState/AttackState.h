#pragma once
#include "PlayerState.h"

class AttackState : public PlayerState
{
public:
    AttackState(Player* p);

    void Enter() override;
    void Update(float dt) override;
    void Exit() override;

private:
    int comboStep = 0;
    float timer = 0;
    bool hasHit = false;

    void NextAttack();
};