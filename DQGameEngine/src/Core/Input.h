#pragma once
#include <SDL3/SDL.h>
#include "Rect.h"

class INPUT
{
private:
	SDL_Event event;
	bool onRelease;
	bool quit;
	void CopyKey();
public:
	Rect mouseRect;
	INPUT() {}
	void Update();
	bool Quit() { return quit; }
};

extern INPUT Input;
extern const bool* Key;
extern Uint8 PreKey[SDL_SCANCODE_COUNT];