#pragma once
#include <iostream>
#include <string>
#include "RenderWindow.h"
#include "Graphics/Graphics.h"
class WindowContainer
{
public:
	WindowContainer();
	 LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	RenderWindow render_window;
	Graphics gamefx;
private:

};