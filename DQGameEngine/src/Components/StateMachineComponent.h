#pragma once
#include "Component.h"
#include "Core/IState.h"
#include "Core/StateMachine.h"

class StateMachineComponent : public Component
{
public:
    void ChangeState(IState* state);
protected:
    void OnUpdate(float dt) override;
private:
    StateMachine fsm;
    
};

