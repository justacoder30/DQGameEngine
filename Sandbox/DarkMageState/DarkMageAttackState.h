#pragma once
#include "DarkMageState.h"
class DarkMageAttackState : public DarkMageState {
public:
    using DarkMageState::DarkMageState;
    void Enter() override;
    void Update(float dt) override;
private:
    bool hasCast = false;
};
class DarkMageAttack2State : public DarkMageState {
public:
    using DarkMageState::DarkMageState;
    void Enter() override;
    void Update(float dt) override;
private:
    int shots = 0;
    bool doubleShot = false;
};
class DarkMageHealState : public DarkMageState {
public:
    using DarkMageState::DarkMageState;
    void Enter() override;
    void Update(float dt) override;
    void Exit() override;
private:
    bool healed = false;
};
