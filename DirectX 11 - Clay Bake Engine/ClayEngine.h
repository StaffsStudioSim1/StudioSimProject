#pragma once
#include "WindowContainer.h"
#include "Examples.h"
#include "Scene.h"
#include "GameObjects/Physics.h"
#include "PhysicsStructs.h"

#define FPS_5 1.0f/5.0f
#define FPS_30 1.0f/30.0f
#define FPS_60 1.0f/60.0f
#define FPS_CAP FPS_60

#define FIXED_TIMESTEP 1.0f/50.0f

class ClayEngine : WindowContainer
{

public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	void Destroy();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
protected:

private:
	bool _initialised = false;

	// DO NOT SET DIRECTLY - Use SceneManager::GetInstance().LoadScene("file/path.json");
	Scene* _scene = nullptr;
	Examples* _ex;
	PhysicsWorld* _physicsWorld;
	Physics* _physicsAccess;
	bool _physicsRunning;
};