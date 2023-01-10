#include "ClayEngine.h"
#include "Input/InputManager.h"

bool ClayEngine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}
	if (!gamefx.Initialize(this->render_window.GetHWND(), width, height))
	{
		return false;
	}

	// Initialise Input Manager
	InputManager::GetInstance();
	_testInput = InputManager::GetInstance().GetNewPlayerInput();
	_testInput->SetDeviceType(KeyboardLeft);
	// initialise graphics here
	return true;
}

LRESULT ClayEngine::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = InputManager::GetInstance().WindowProc(uMsg, wParam, lParam);
	if (result == -1)
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	return result;
}

bool ClayEngine::ProcessMessages()
{
	return this->render_window.proccessMessages();
}

void ClayEngine::Update()
{
	InputManager::GetInstance().PollInput();
	//InputManager::GetInstance().Debug();
	if (_testInput->IsActionDown(Jump))
		OutputDebugStringA("Jump Pressed\n");
	if (_testInput->IsActionHeld(Jump))
		OutputDebugStringA("Jump Held\n");
	if (_testInput->IsActionUp(Jump))
		OutputDebugStringA("Jump Released\n");
}

void ClayEngine::RenderFrame()
{
	gamefx.RenderFrame();
}