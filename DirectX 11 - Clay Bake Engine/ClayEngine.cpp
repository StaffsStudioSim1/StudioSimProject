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

	// Physics world for data processing
	b2World boxWorld(b2Vec2(0.0f, -9.81f));
	//_PhysicsWoldSimulation = new _mPhysicsWorld();
	//->_world = boxWorld;

	// initialise graphics here

	_initialised = true;
#if EDIT_MODE
	_editor = new Scene("Resources/demo.json");
#else
	SceneManager::GetInstance().LoadScene("Resources/demo.json");
#endif
	return true;
}

void ClayEngine::Destroy()
{
	delete _ex;
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
#if EDIT_MODE
	if (_editor != nullptr)
		_editor->Update(0);
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
#endif
}

void ClayEngine::RenderFrame()
{
#if EDIT_MODE
  if (_editor != nullptr)
    gamefx.Render(_editor)
#else
	if (_scene != nullptr)
		gamefx.RenderFrame(_scene);
#endif
}
