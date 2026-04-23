#include "pch.h"
#include "StateMachineComponent.h"

void StateMachineComponent::ChangeState(IState* state)
{
    fsm.ChangeState(state);
}

void StateMachineComponent::OnUpdate(float dt)
{
    fsm.Update(dt);
}
