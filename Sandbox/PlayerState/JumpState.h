#pragma once
#include "PlayerState.h"

class JumpState : public PlayerState
{
public:
    using PlayerState::PlayerState;

    void Enter() override;    

    void Update(float dt) override;
};///