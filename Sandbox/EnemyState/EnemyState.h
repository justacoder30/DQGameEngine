#pragma once
#include "DQEngine.h"

class Skeleton;

class EnemyState : public IState
{
public:
    EnemyState(Skeleton* e) : enemy(e) {}

protected:
    Skeleton* enemy;
};
