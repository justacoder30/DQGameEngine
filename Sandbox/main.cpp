#include "DQEngine.h"
#include "GameTest.h"
#include <iostream>

int main(int argc, char** argv) {
	GameTest game(1920, 1080, "DQ Game Engine - Sandbox");
	game.Run();
    return 0;
}
