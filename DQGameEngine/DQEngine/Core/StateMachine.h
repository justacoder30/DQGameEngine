#pragma once
#include "IState.h"

namespace dqengine {

class StateMachine
{
public:
    void ChangeState(IState* newState);

    void Update(float dt);

private:
    IState* current = nullptr;
};


} // namespace dqengine
