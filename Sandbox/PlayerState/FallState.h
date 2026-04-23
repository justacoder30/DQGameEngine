#pragma once
#include "PlayerState.h"

class FallState : public PlayerState
{
public:
    using PlayerState::PlayerState;

    void Enter() override;

    void Update(float dt) override;
};
