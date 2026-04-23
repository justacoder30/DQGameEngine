#pragma once
#include "IState.h"

class StateMachine
{
public:
    void ChangeState(IState* newState);

    void Update(float dt);

private:
    IState* current = nullptr;
};

