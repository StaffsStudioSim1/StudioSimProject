#include "PlayerInput.h"
#include "InputManager.h"

PlayerInput::PlayerInput(int id)
{
	_id = id;
	FillButtonMap(_actionDown);
	FillButtonMap(_actionHeld);
	FillAxisMap(_axisMap);
	SetDeviceType(Unassigned);
	InputManager::GetInstance().RegisterPlayerInput(this);
}

PlayerInput::~PlayerInput()
{
	InputManager::GetInstance().UnregisterPlayerInput(this);
}

void PlayerInput::PollInput(KeyboardClass keyboard)
{
	if (_deviceType != KeyboardLeft && _deviceType != KeyboardRight)
		return;

	// Movement
	Vector2 axis;
	if (keyboard.IsKeyPressed(_actionMap.leftKeyCode))
		axis.x -= 1;
	// Both keys can be pressed to equal each other out, so do not change to 'else if'
	if (keyboard.IsKeyPressed(_actionMap.rightKeyCode))
		axis.x += 1;
	SetValue(_axisMap, Movement, axis);
	// End Movement

	// Jump
	// Is the button down?
	if (keyboard.IsKeyPressed(_actionMap.jumpKeyCode))
	{
		// Is the button not being held?
		if (!GetValue(_actionHeld, Jump))
		{
			// This code sets Held to true the second frame the button is down
			SetValue(_actionHeld, Jump, GetValue(_actionDown, Jump));
			SetValue(_actionDown, Jump, true);
		}
	}
	// Was the button down last frame?
	else if (GetValue(_actionDown, Jump))
	{
		// Set the state so that IsActionUp will return true
		SetValue(_actionDown, Jump, false);
		SetValue(_actionHeld, Jump, true);
	}
	// Is the button in IsActionUp state?
	else if (GetValue(_actionHeld, Jump))
	{
		SetValue(_actionDown, Jump, false);
		SetValue(_actionHeld, Jump, false);
	}

	// Interact
	if (keyboard.IsKeyPressed(_actionMap.interactKeyCode))
	{
		if (!GetValue(_actionHeld, Interact))
		{
			SetValue(_actionHeld, Interact, GetValue(_actionDown, Interact));
			SetValue(_actionDown, Interact, true);
		}
	}
	else if (GetValue(_actionDown, Interact))
	{
		SetValue(_actionDown, Interact, false);
		SetValue(_actionHeld, Interact, true);
	}
	else if (GetValue(_actionHeld, Interact))
	{
		SetValue(_actionDown, Interact, false);
		SetValue(_actionHeld, Interact, false);
	}

	// Magnet
	if (keyboard.IsKeyPressed(_actionMap.magnetKeyCode))
	{
		if (!GetValue(_actionHeld, Magnet))
		{
			SetValue(_actionHeld, Magnet, GetValue(_actionDown, Magnet));
			SetValue(_actionDown, Magnet, true);
		}
	}
	else if (GetValue(_actionDown, Magnet))
	{
		SetValue(_actionDown, Magnet, false);
		SetValue(_actionHeld, Magnet, true);
	}
	else if (GetValue(_actionHeld, Magnet))
	{
		SetValue(_actionDown, Magnet, false);
		SetValue(_actionHeld, Magnet, false);
	}

	// Pause
	if (keyboard.IsKeyPressed(_actionMap.pauseKeyCode))
	{
		if (!GetValue(_actionHeld, Pause))
		{
			SetValue(_actionHeld, Pause, GetValue(_actionDown, Pause));
			SetValue(_actionDown, Pause, true);
		}
	}
	else if (GetValue(_actionDown, Pause))
	{
		SetValue(_actionDown, Pause, false);
		SetValue(_actionHeld, Pause, true);
	}
	else if (GetValue(_actionHeld, Pause))
	{
		SetValue(_actionDown, Pause, false);
		SetValue(_actionHeld, Pause, false);
	}
}

bool PlayerInput::IsActionDown(Action action)
{
	return GetValue(_actionDown, action) && !GetValue(_actionHeld, action);
}

bool PlayerInput::IsActionHeld(Action action)
{
	return GetValue(_actionDown, action) && GetValue(_actionHeld, action);
}

bool PlayerInput::IsActionUp(Action action)
{
	return !GetValue(_actionDown, action) && GetValue(_actionHeld, action);
}

Vector2 PlayerInput::ReadAxis(Action action)
{
	return GetValue(_axisMap, action);
}

DeviceType PlayerInput::GetDeviceType()
{
	return _deviceType;
}

void PlayerInput::SetDeviceType(DeviceType deviceType)
{
	_deviceType = deviceType;
	ResetButtonMap(_actionDown);
	ResetButtonMap(_actionHeld);
	ResetAxisMap(_axisMap);

	// Scan Codes from https://learn.microsoft.com/en-gb/windows/win32/inputdev/about-keyboard-input#scan-codes
	// NOTE: Those values start at 1, therefore we need to subtract 1 when putting them here. E.G. w on the docs is 18, but the code says it's 17
	switch (deviceType)
	{
	case KeyboardLeft: // A, D, W, E, Q, Esc
		_actionMap.leftKeyCode = 30;
		_actionMap.rightKeyCode = 32;
		_actionMap.jumpKeyCode = 17;
		_actionMap.interactKeyCode = 18;
		_actionMap.magnetKeyCode = 16;
		_actionMap.pauseKeyCode = 109;
		break;
	case KeyboardRight: // Left, Right, Up, RControl, RShift, Esc
		_actionMap.leftKeyCode = 78;
		_actionMap.rightKeyCode = 87;
		_actionMap.jumpKeyCode = 82;
		_actionMap.interactKeyCode = 63;
		_actionMap.magnetKeyCode = 56;
		_actionMap.pauseKeyCode = -1;
		break;
	case Unassigned:
	case Controller:
		_actionMap.leftKeyCode = -1;
		_actionMap.rightKeyCode = -1;
		_actionMap.jumpKeyCode = -1;
		_actionMap.interactKeyCode = -1;
		_actionMap.magnetKeyCode = -1;
		_actionMap.pauseKeyCode = -1;
		break;
	}
}

int PlayerInput::GetID()
{
	return _id;
}

bool PlayerInput::GetValue(std::map<Action, bool>& map, Action action)
{
	std::map<Action, bool>::iterator it = map.find(action);
	if (it != map.end())
		return it->second;
	return false;
}

Vector2 PlayerInput::GetValue(std::map<Action, Vector2>& map, Action action)
{
	std::map<Action, Vector2>::iterator it = map.find(action);
	if (it != map.end())
		return it->second;
	return Vector2(-INFINITY, -INFINITY);
}

void PlayerInput::FillButtonMap(std::map<Action, bool>& map)
{
	map.insert(std::make_pair(Jump, false));
	map.insert(std::make_pair(Interact, false));
	map.insert(std::make_pair(Magnet, false));
	map.insert(std::make_pair(Pause, false));
}

void PlayerInput::ResetButtonMap(std::map<Action, bool>& map)
{
	SetValue(map, Jump, false);
	SetValue(map, Interact, false);
	SetValue(map, Magnet, false);
	SetValue(map, Pause, false);
}

void PlayerInput::FillAxisMap(std::map<Action, Vector2>& map)
{
	map.insert(std::make_pair(Movement, Vector2()));
}

void PlayerInput::ResetAxisMap(std::map<Action, Vector2>& map)
{
	SetValue(map, Movement, Vector2());
}
