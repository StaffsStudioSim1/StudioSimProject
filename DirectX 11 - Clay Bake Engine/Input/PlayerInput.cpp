#include "PlayerInput.h"

PlayerInput::PlayerInput()
{
	FillButtonMap(_actionDown);
	FillButtonMap(_actionHeld);
	SetDeviceType(Unassigned);
}

void PlayerInput::PollInput(KeyboardClass keyboard)
{
	// Jump
	if (_deviceType == KeyboardLeft || _deviceType == KeyboardRight)
	{
		if (keyboard.IsKeyPressed(_actionMap.jumpKeyCode))
		{
			if (!GetValue(_actionHeld, Jump))
			{
				SetValue(_actionHeld, Jump, GetValue(_actionDown, Jump));
				SetValue(_actionDown, Jump, true);
			}
		}
		else if (GetValue(_actionDown, Jump))
		{
			SetValue(_actionDown, Jump, false);
			SetValue(_actionHeld, Jump, true);
		}
		else
		{
			SetValue(_actionDown, Jump, false);
			SetValue(_actionHeld, Jump, false);
		}
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

DeviceType PlayerInput::GetDeviceType()
{
	return _deviceType;
}

void PlayerInput::SetDeviceType(DeviceType deviceType)
{
	_deviceType = deviceType;
	ResetButtonMap(_actionDown);
	ResetButtonMap(_actionHeld);

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
		_actionMap.jumpKeyCode =-1;
		_actionMap.interactKeyCode = -1;
		_actionMap.magnetKeyCode = -1;
		_actionMap.pauseKeyCode = -1;
		break;
	}
}

bool PlayerInput::GetValue(std::map<Action, bool>& map, Action action)
{
	std::map<Action, bool>::iterator it = map.find(action);
	if (it != map.end())
		return it->second;
	return false;
}

void PlayerInput::SetValue(std::map<Action, bool>& map, Action action, bool value)
{
	std::map<Action, bool>::iterator it = map.find(action);
	if (it != map.end())
		it->second = value;
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
