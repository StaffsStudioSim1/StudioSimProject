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
	HWND handle = NULL;			// window handle
	HINSTANCE hInstance = NULL; // application instance handle 
	std::string window_title = ""; 
	std::wstring window_title_wide = L"";// wide ver for windows interaction 
	std::string window_class = ""; 
	std::wstring window_class_wide = L"";// wide ver for windows interaction 
	int width = 0;
	int height = 0;

};