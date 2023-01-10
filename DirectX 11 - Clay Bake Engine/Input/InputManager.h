#pragma once
#include "Keyboard/KeyboardClass.h"
#include "Mouse/MouseClass.h"
#include "ActionMap.h"
#include <windows.h>

enum DeviceType { KeyboardLeft, KeyboardRight, Controller };

class InputManager
{
public:
	// Singleton Code
	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;

	// Process Windows Proc, returns -1 if not handled
	LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Debug();
	// Update ActionMaps
	void PollInput();
private:
	InputManager();
	~InputManager();

	KeyboardClass _keyboard;
	MouseClass _mouse;

	std::vector<ActionMap> _actionMaps;
};

