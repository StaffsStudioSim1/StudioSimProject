#pragma once
#include <map>
#include "Keyboard/KeyboardClass.h"
#include "ActionMap.h"
#include "../Physics/Vectors.h"

enum DeviceType { Unassigned, KeyboardLeft, KeyboardRight, Controller };
enum Action { Movement, Jump, Interact, Magnet, Pause };

class PlayerInput
{
public:
	PlayerInput(int id);
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

	// Read the Vector2 Data from an Action
	Vector2 ReadAxis(Action action);

	// Get the DeviceType this is bound to
	DeviceType GetDeviceType();
	// Bind this to the specified DeviceType
	void SetDeviceType(DeviceType deviceType);

	int GetID();
private:
	int _id;
	DeviceType _deviceType;
	ActionMap _actionMap;

	std::map<Action, bool> _actionDown;
	std::map<Action, bool> _actionHeld;
	std::map<Action, Vector2> _axisMap;

	// Useful functions

	static bool GetValue(std::map<Action, bool>& map, Action action);
	static Vector2 GetValue(std::map<Action, Vector2>& map, Action action);

	template<typename T>
	static void SetValue(std::map<Action, T>& map, Action action, T value)
	{
		typename std::map<Action, T>::iterator it = map.find(action);
		if (it != map.end())
			it->second = value;
	}

	static void FillButtonMap(std::map<Action, bool>& map);
	static void ResetButtonMap(std::map<Action, bool>& map);
	static void FillAxisMap(std::map<Action, Vector2>& map);
	static void ResetAxisMap(std::map<Action, Vector2>& map);
};

