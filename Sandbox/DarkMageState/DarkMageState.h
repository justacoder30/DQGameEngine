#pragma once
#include "../Engine.h"

class DarkMage;

class DarkMageState : public IState
{
public:
    DarkMageState(DarkMage* b) : boss(b) { }

protected:
    DarkMage* boss;
};

