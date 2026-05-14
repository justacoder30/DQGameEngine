#pragma once
#include "PlayerState.h"

class RollState : public PlayerState
{
public:
    RollState(Player* p);

    void Enter() override;
    void Update(float dt) override;
    void Exit() override;

private:
    float timer = 0;

    float duration = 0.35f;
    float rollSpeed = 420.f;

    int dir = 1;
};