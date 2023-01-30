#pragma once
#include <map>
#include "Keyboard/KeyboardClass.h"
#include "ActionMap.h"

enum DeviceType { Unassigned, KeyboardLeft, KeyboardRight, Controller };
enum Action { Movement, Jump, Interact, Magnet, Pause };

class PlayerInput
{
public:
	PlayerInput();
	~PlayerInput();

	// Only call from InputManager
	void PollInput(KeyboardClass keyboard);
	// Only call from InputManager
	//void PollInput(ControllerClass keyboard); // TODO

	// Returns true on the first frame the Action is pressed
	bool IsActionDown(Action action);
	// Returns true on the second and following frames the Action is held down
	bool IsActionHeld(Action action);
	// Returns true on the frame the Action is released
	bool IsActionUp(Action action);

	// Get the DeviceType this is bound to
	DeviceType GetDeviceType();
	// Bind this to the specified DeviceType
	void SetDeviceType(DeviceType deviceType);
private:
	DeviceType _deviceType;
	ActionMap _actionMap;

	std::map<Action, bool> _actionDown;
	std::map<Action, bool> _actionHeld;

	// Useful functions
	static bool GetValue(std::map<Action, bool>& map, Action action);
	static void SetValue(std::map<Action, bool>& map, Action action, bool value);
	static void FillButtonMap(std::map<Action, bool>& map);
	static void ResetButtonMap(std::map<Action, bool>& map);
};

