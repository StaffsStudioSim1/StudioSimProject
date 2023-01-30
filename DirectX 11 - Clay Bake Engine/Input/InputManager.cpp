#include "InputManager.h"
#include <memory>
#include <string>
#include <debugapi.h>

InputManager::InputManager()
{
	_keyboard.EnableAutoRepeatChars(); // ENABLES char auto repeat for output example of default class use 
}

InputManager::~InputManager()
{
	_inputs.clear();
}

LRESULT InputManager::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{ // Keyboard Messages
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		unsigned int scanCode = (lParam >> 16) & 0xff;
		if (_keyboard.IsKeyAutoRepeat())
			_keyboard.OnKeyPressed(keycode);
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
				_keyboard.OnKeyPressed(keycode);
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char> (wParam);
		_keyboard.OnKeyReleased(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (_keyboard.IsCharAutoRepeat())
			_keyboard.OnChar(ch);
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
				_keyboard.OnChar(ch);
		}
		return 0;
	} // Mouse Messages 
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		_mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			_mouse.OnWheelUp(x, y);
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			_mouse.OnWheelDown(x, y);
		return 0;
	}
	case WM_INPUT:
	{
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
					_mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			}
		}

		return -1;
	}
	default:
		return -1;
	}
}

void InputManager::Debug()
{
	while (!_keyboard.IsCharBufferEmpty())
	{	// way to output current char pressed	shows just letter pressed
		unsigned char ch = _keyboard.ReadChar();	 // read key
		std::string outmsg = "char: "; outmsg += ch; outmsg += "\n"; // compile complete string 
		OutputDebugStringA(outmsg.c_str());	// output string 
	}

	while (!_keyboard.IsKeyBufferEmpty())
	{	// way to output current Key pressed shows key event of down / up
		KeyboardEvent kbe = _keyboard.ReadKey(); // read keyboard event
		unsigned char keycode = kbe.GetKeyCode();	// get key pressed
		std::string outmsg = "key: "; outmsg += keycode; outmsg += "\n"; // compile string for output
		OutputDebugStringA(outmsg.c_str()); // output string 
	}

	while (!_mouse.EventBufferIsEmpty())
	{
		MouseEvent me = _mouse.ReadEvent(); // read mouse event the mouse moment here is just basic pos on screen 
		std::string outmsg = "X:"; outmsg += std::to_string(me.GetPosX()); outmsg += ", Y: "; outmsg += std::to_string(me.GetPosY()); outmsg += "\n";	// read mouse event the mouse moment here is
		OutputDebugStringA(outmsg.c_str()); // output string																					//  just basic pos on screen then compile mouse string for output
		if (me.GetType() == MouseEvent::EventType::WheelUp)
			OutputDebugStringA("mouse wheel up.\n");
		else if (me.GetType() == MouseEvent::EventType::WheelDown) // scroll wheel examples
			OutputDebugStringA("mouse wheel down.\n");
		else if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			std::string outmsg = "X: "; outmsg += std::to_string(me.GetPosX()); outmsg += ", Y: "; outmsg += std::to_string(me.GetPosY()); outmsg += "\n"; // this string settup uses raw mouse movment
			OutputDebugStringA(outmsg.c_str());
		}
	}
}

void InputManager::RegisterPlayerInput(PlayerInput* playerInput)
{
	_inputs.push_back(playerInput);
}

void InputManager::UnregisterPlayerInput(PlayerInput* playerInput)
{
	_inputs.erase(std::remove(_inputs.begin(), _inputs.end(), playerInput), _inputs.end());
}

void InputManager::PollInput()
{
	for (PlayerInput* map : _inputs)
	{
		switch (map->GetDeviceType())
		{
		case DeviceType::KeyboardLeft:
		case DeviceType::KeyboardRight:
			map->PollInput(_keyboard);
			break;
		case DeviceType::Unassigned:
		case DeviceType::Controller:
			break;
		}
	}
}

MouseClass InputManager::GetMouse()
{
	return _mouse;
}
