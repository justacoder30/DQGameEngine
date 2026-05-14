#pragma once
#include "PlayerState.h"

class AirAttackEndState : public PlayerState
{
public:
    using PlayerState::PlayerState;

    void Enter() override;
    void Update(float dt) override;
	void Exit() override;

private:
};

