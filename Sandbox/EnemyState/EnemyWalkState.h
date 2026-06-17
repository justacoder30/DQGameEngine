#pragma once
#include "EnemyState.h"

class EnemyWalkState : public EnemyState
{
public:
    using EnemyState::EnemyState;

    void Enter() override;
    void Update(float dt) override;

private:
    float walkTime = 0.0f;
};
