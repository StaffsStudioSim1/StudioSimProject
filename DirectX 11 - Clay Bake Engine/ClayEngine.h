#pragma once
#include "WindowContainer.h"
#include "Examples.h"
#include "Scene.h"
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
};