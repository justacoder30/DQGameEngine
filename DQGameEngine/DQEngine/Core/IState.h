#pragma once

namespace dqengine {

class IState
{
public:
    virtual ~IState() = default;
    virtual void Enter() {}
    virtual void Update(float dt) {}
    virtual void Exit() {}
};

} // namespace dqengine
