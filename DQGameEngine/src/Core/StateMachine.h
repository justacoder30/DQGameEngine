#pragma once
#include "State.h"

class StateMachine
{
public:
    void ChangeState(State* newState);

    void Update(float dt);

private:
    State* current = nullptr;
};

