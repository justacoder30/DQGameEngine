#include "pch.h"
#include "StateMachine.h"

void StateMachine::ChangeState(IState* newState)
{
    if (current)
        current->Exit();

    current = newState;

    if (current)
        current->Enter();
}

void StateMachine::Update(float dt)
{
    if (current)
        current->Update(dt);
}
