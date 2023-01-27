#pragma once
#include "Keyboard/KeyboardClass.h"
#include "Mouse/MouseClass.h"
#include "PlayerInput.h"
#include <windows.h>

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
	void RegisterPlayerInput(PlayerInput* playerInput);
	void UnregisterPlayerInput(PlayerInput* playerInput);
	void PollInput();

	std::vector<int> PollMouseInput();

	MouseClass GetMouse();
private:
	InputManager();
	~InputManager();

	KeyboardClass _keyboard;
	MouseClass _mouse;

	std::vector<PlayerInput*> _inputs;
};

