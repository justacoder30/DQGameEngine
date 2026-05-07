#pragma once
#include "EnemyState.h"
class EnemyChaseState : public EnemyState
{
public:
    using EnemyState::EnemyState;

    void Enter() override;
    void Update(float dt) override;
};

