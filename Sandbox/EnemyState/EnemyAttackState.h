#pragma once
#include "EnemyState.h"

class EnemyAttackState : public EnemyState
{
public:
    using EnemyState::EnemyState;

    void Enter() override;
    void Update(float dt) override;

private:

	bool hasHit = false;
};

