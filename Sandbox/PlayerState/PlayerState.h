#pragma once
#include "DQEngine/DQEngine.h"
//#include "Player.h"

class Player;

class PlayerState : public IState
{
public:
    PlayerState(Player* p) : player(p) {}
protected:
    Player* player;
};