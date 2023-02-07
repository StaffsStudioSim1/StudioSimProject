#include "ClayEngine.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "SceneManager.h"
#include "GameObjects/ObjectHandler.h"


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

	// Physics world for data processing
	float gravity = -9.806f;
	b2World* boxworld = new b2World(b2Vec2(0, gravity));
	_physicsWorld = new PhysicsWorld();
	_physicsWorld->world = boxworld;// = *_physicsAccess->CreatePhysicsWorld(gravity);
	ObjectHandler::GetInstance().SetPhysicsWorld(_physicsWorld);

	// initialise graphics here

	_initialised = true;
	SceneManager::GetInstance().LoadScene("Resources/MainMenu.json");
	return true;
}

void ClayEngine::Destroy()
{
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

	// Example message for object collision
	if (ObjectHandler::GetInstance().GetAllObjects().size() > 2)
	{
		if (ObjectHandler::GetInstance().GetGameObject(1)->GetComponent<Physics>() && ObjectHandler::GetInstance().GetGameObject(2)->GetComponent<Physics>()) // Checks both objects have physics
		{
			if (ObjectHandler::GetInstance().GetGameObject(1)->GetComponent<Physics>()->IsObjectCollidingwith(*ObjectHandler::GetInstance().GetGameObject(2)->GetComponent<Physics>()->GetPhysicsBody()))
			{
				OutputDebugStringA("obj 1 and 2 have collided \n");
			}
		}
	}

	dwTimeStart = dwTimeCur;
#endif

	if (SceneManager::GetInstance().ShouldSceneChange())
	{
		if (_scene != nullptr)
			_scene->Stop();
		_scene = SceneManager::GetInstance().ReadScene();
		_scene->Start();
	}


}

void ClayEngine::RenderFrame()
{
	if (_scene)
		gamefx.RenderFrame(_scene);
}
