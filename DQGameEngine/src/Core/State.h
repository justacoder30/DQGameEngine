#pragma once


class State
{
public:
    virtual ~State() = default;
    virtual void Enter() {}
    virtual void Update(float dt) {}
    virtual void Exit() {}
};
