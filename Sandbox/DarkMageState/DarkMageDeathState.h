#pragma once
#include "DarkMageState.h"
class DarkMageDeathState : public DarkMageState
{
    using DarkMageState::DarkMageState;

    void Enter() override;
    void Update(float dt) override;
};

