#pragma once
#include "WindowContainer.h"
#include "Examples.h"
#include "Scene.h"
#include "PhysicsStructs.h"

#define FPS_5 1.0f/5.0f
#define FPS_30 1.0f/30.0f
#define FPS_60 1.0f/60.0f
#define FPS_CAP FPS_60

#define FIXED_TIMESTEP 1.0f/50.0f

#define EDIT_MODE false

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

	Scene* _scene = nullptr;
	Examples* _ex;
	PhysicsWorld* _PhysicsWorld;
	Physics* _physicsAccess;
	bool _physicsRunning;

#if EDIT_MODE
	SceneEditor* _editor;
#endif
};