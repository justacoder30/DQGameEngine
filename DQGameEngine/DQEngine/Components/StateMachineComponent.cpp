#include "pch.h"
#include "StateMachineComponent.h"

namespace dqengine {

void StateMachineComponent::ChangeState(IState* state)
{
    fsm.ChangeState(state);
}

void StateMachineComponent::OnUpdate(float dt)
{
    fsm.Update(dt);
}

} // namespace dqengine
