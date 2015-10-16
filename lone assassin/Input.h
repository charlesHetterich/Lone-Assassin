#pragma once
#include "globalVariablesAndIncludes.h"

const static int NUM_KEYCODES = 256;
const static int NUM_MOUSECODES = 3;

class Input
{
public:
	Input();
	~Input();

	//keyboard
	bool getKey(int keyCode);
	bool getKeyDown(int keyCode);
	bool getKeyUp(int keyCode);

	//mouse
	int getMouseX();
	int getMouseY();
	bool getMouseButton(int button);
	bool getMouseButtonDown(int button);
	bool getMouseButtonUp(int button);

	//update
	void update();
private:

	//keyboard
	bool keyIsPressed[NUM_KEYCODES];
	bool keyJustDown[NUM_KEYCODES];
	bool keyJustUp[NUM_KEYCODES];
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//mouse
	int button;
	int mouseX;
	int mouseY;
	bool mouseButtonIsPressed[NUM_MOUSECODES];
	bool mouseButtonJustDown[NUM_MOUSECODES];
	bool mouseButtonJustUp[NUM_MOUSECODES];
};