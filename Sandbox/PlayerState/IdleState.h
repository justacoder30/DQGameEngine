#pragma once
#include "PlayerState.h"

class IdleState : public PlayerState
{
public:
    using PlayerState::PlayerState;

    void Enter() override;

    void Update(float dt) override;
};