#pragma once
#include "DarkMageState.h"

class DarkMageHurtState : public DarkMageState
{
    using DarkMageState::DarkMageState;

    void Enter() override;
    void Update(float dt) override;
};

