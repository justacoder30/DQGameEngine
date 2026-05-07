#pragma once
#include "PlayerState.h"


class HurtState : public PlayerState
{
public:
    using PlayerState::PlayerState;

    void Enter() override;

    void Update(float dt) override;
};

