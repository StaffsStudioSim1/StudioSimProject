#include "ClayEngine.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "SceneManager.h"

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

	// Initialise Audio Engine
	AudioManager::GetInstance();

	_ex = new Examples();

	// initialise graphics here

	_initialised = true;
	_scene = new Scene("Resources/scene_demo.json");
	return true;
}

void ClayEngine::Destroy()
{
	delete _ex;
	if (_scene)
		delete _scene;
}

LRESULT ClayEngine::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!_initialised)
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	LRESULT result = InputManager::GetInstance().WindowProc(uMsg, wParam, lParam);

	if (result != -1)
		return result;

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool ClayEngine::ProcessMessages()
{
	return this->render_window.proccessMessages();
}

void ClayEngine::Update()
{
	AudioManager::GetInstance().Update();

	static float deltaTime = 0.0f;
	static DWORD dwTimeStart = 0;

	static float deltaTimeFixed = 0.0f;
	static DWORD dwTimeStartFixed = 0;

	DWORD dwTimeCur = GetTickCount64();

	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	if (dwTimeStartFixed == 0)
		dwTimeStartFixed = dwTimeCur;

	deltaTime = (dwTimeCur - dwTimeStart) / 1000.0f;
	deltaTimeFixed = (dwTimeCur - dwTimeStartFixed) / 1000.0f;

	if (deltaTimeFixed >= FIXED_TIMESTEP)
	{
		while (deltaTimeFixed >= FIXED_TIMESTEP)
		{
			if (_scene != nullptr)
				_scene->FixedUpdate(FIXED_TIMESTEP);

			deltaTimeFixed -= FIXED_TIMESTEP;
		}

		dwTimeStartFixed = dwTimeCur;
	}

	if (deltaTime < FPS_CAP)
		return;
	InputManager::GetInstance().PollInput();

	_ex->Update();
	if (_scene != nullptr)
		_scene->Update(deltaTime);

	if (SceneManager::GetInstance().ShouldSceneChange())
	{
		if (_scene != nullptr)
			_scene->Stop();
		_scene = SceneManager::GetInstance().ReadScene();
		_scene->Start();
	}

	dwTimeStart = dwTimeCur;
}

void ClayEngine::RenderFrame()
{
	gamefx.RenderFrame(_scene);
}
