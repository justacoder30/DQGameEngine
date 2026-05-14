#pragma once
#include "DarkMageState.h"
class DarkMageChaseState : public DarkMageState
{
public:

    using DarkMageState::DarkMageState;

    void Enter() override;
    void Update(float dt) override;
};

