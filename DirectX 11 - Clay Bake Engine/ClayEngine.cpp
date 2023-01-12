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

	// Guarantee InputManager is initialised at this point
	InputManager::GetInstance();

	// initialise graphics here

	_initialised = true;
	return true;
}

void ClayEngine::Destroy()
{

}

LRESULT ClayEngine::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!_initialised)
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

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
}

void ClayEngine::RenderFrame()
{
	gamefx.RenderFrame();
}