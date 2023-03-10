#include "ClayEngine.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "SceneManager.h"
#include "GameObjects/ObjectHandler.h"
#include "Defines.h"


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
	_width = width;
	_height = height;

	// Guarantee InputManager is initialised at this point
	InputManager::GetInstance();

	// Initialise Audio Engine
	AudioManager::GetInstance();

	// Guarantee GameManager is initialised
	GameManager::GetInstance();

	SceneManager::GetInstance().Initialise(width, height);

	// initialise graphics here

	_initialised = true;
#if EDIT_MODE
	SceneManager::GetInstance().LoadScene("Resources/Blank.json");
#else
	SceneManager::GetInstance().LoadScene("Resources/MainMenu.json");
#endif
	return true;
}

void ClayEngine::Destroy()
{
	if (_scene)
		delete _scene;
	AudioManager::GetInstance().Kill();
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
	return this->render_window.proccessMessages() && _running;
}

void ClayEngine::Update()
{
	if (gamefx.kill)
	{
		_running = false;
		return;
	}
#if EDIT_MODE
	if (_scene != nullptr)
		_scene->Update(0);
#else
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

	if (_scene != nullptr)
		_scene->Update(deltaTime);



	dwTimeStart = dwTimeCur;
#endif

	if (SceneManager::GetInstance().ShouldSceneChange())
	{
		if (_scene != nullptr)
		{
			_scene->Stop();
			delete _scene;
		}
		_scene = SceneManager::GetInstance().ReadScene();
		GameManager::GetInstance().SceneChanged(_scene);
		_scene->Start();
	}


}

void ClayEngine::RenderFrame()
{
	if (_scene)
		gamefx.RenderFrame(_scene);
}
