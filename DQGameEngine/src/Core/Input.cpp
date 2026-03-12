#include "pch.h"
#include "Input.h"

INPUT Input = INPUT();
const bool* Key = SDL_GetKeyboardState(NULL);
Uint8 PreKey[SDL_SCANCODE_COUNT];

void INPUT::CopyKey()
{
	for (int i = 0; i < SDL_SCANCODE_COUNT; i++) { // init them all to false
		PreKey[i] = Key[i];
	}
}

void INPUT::Update()
{
	CopyKey();

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			if (event.type == SDL_EVENT_QUIT)
				quit = true;
		}
	}
}

