#pragma once
#include"ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer * pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool proccessMessages();
	HWND GetHWND() const;
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND _handle = NULL;			// window handle
	HINSTANCE _hInstance = NULL; // application instance handle
	std::string _windowTitle = ""; 
	std::wstring _windowTitleWide = L"";// wide ver for windows interaction 
	std::string _windowClass = ""; 
	std::wstring _windowClassWide = L"";// wide ver for windows interaction 
	int _width = 0;
	int _height = 0;

};