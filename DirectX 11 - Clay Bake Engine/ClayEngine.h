#pragma once
#include "WindowContainer.h"
class ClayEngine : WindowContainer
{

public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
protected:

private:

};