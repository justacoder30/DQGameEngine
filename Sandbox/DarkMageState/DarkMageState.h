#pragma once
#include "DQEngine/DQEngine.h"

class DarkMage;

class DarkMageState : public IState
{
public:
    DarkMageState(DarkMage* b) : boss(b) { }

protected:
    DarkMage* boss;
};

