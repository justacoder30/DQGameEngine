#pragma once
#include "DarkMageState.h"

class DarkMageAttackState : public DarkMageState
{
public:
    using DarkMageState::DarkMageState;

    void Enter() override;
    void Update(float dt) override;

private: 
	bool hasCast = false;
	float timer = 0;
};

