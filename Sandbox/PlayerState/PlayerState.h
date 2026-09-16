#pragma once
#include "../Engine.h"
//#include "Player.h"

class Player;

class PlayerState : public IState
{
public:
    PlayerState(Player* p) : player(p) {}
protected:
    Player* player;
};