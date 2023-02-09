#include"WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(this->window_class);

	this->RegisterWindowClass();
	
	std::ifstream file("Resources/Settings.json");
	if (!file.good())
		ErrorLogger::Log("Unable to find settings file!");

	json data = json::parse(file);
	bool useFullscreen = data["Fullscreen"];
	file.close();

	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(MonitorFromWindow(GetActiveWindow(), MONITOR_DEFAULTTONEAREST), &mi);
	UINT monitorX = mi.rcMonitor.right - mi.rcMonitor.left;
	UINT monitorY = mi.rcMonitor.bottom - mi.rcMonitor.top;

	if (useFullscreen && (monitorX != width || monitorY != height))
		useFullscreen = false;

	int centreOfScreenX = mi.rcMonitor.left + (monitorX / 2 - width / 2);
	int centreOfScreenY = mi.rcMonitor.top + (monitorY / 2 - height / 2) + 30;

	if (useFullscreen)
		centreOfScreenY = mi.rcMonitor.top;

	RECT rc = { centreOfScreenX, centreOfScreenY, centreOfScreenX + width, centreOfScreenY + height };

	AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_POPUP, FALSE);

	this->handle = CreateWindowEx(0, this->window_class_wide.c_str(), this->window_title_wide.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_POPUP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, this->hInstance, pWindowContainer);
							// extended window style, class name, class title, windows style, window X pos, window Y pos, window width, window height, handle parent of window, handle child of window, handle instance, param to create window to be set later
	if (this->handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "CreateWindowEx Failed for window: " + this->window_title);
		return false;
	}

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::proccessMessages()
{	// handle windows msg's 
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));	// init msg structure

	while (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))	// msg pass, window handle, min filer value, max filter value, remove msg after capture
	{
		TranslateMessage(&msg); // translate from virtual key msg to char
		DispatchMessage(&msg);	// dispatch msg to window proc
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL; // msg processing loop takes care of destroying window
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{case WM_CLOSE: 
	{	// other msg's
		DestroyWindow(hwnd);
		return 0;
	}

	default:
	{	// retrieve ptr
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*> (GetWindowLongPtr(hwnd, GWLP_USERDATA));
		// forward msg to window class handler
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
		
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		//OutputDebugStringA("The Window was created. \n");
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);	
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
		{
			ErrorLogger::Log("Critical Error: pointer to window container is null WM_NCCREATE.");
				exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;		// initial window class to be detailed before initialization 
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// flags for redraw width and height to allow for window resize and  movement 
	wc.lpfnWndProc = HandleMessageSetup;						// pointer to window msg handler process
	wc.cbClsExtra = 0;									// Num of additional bytes allocated to window class structure
	wc.cbWndExtra = 0;									// Num of additional bytes allocated to window instance
	wc.hInstance = this->hInstance;						// pass windows instance 
	wc.hIcon = NULL;									// class icon
	wc.hIconSm = NULL;									// small class icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// load default cursor as general point
	wc.hbrBackground = NULL;							// windows application background colour
	wc.lpszMenuName = NULL;								// menus set to null as not currently in use 
	wc.lpszClassName = this->window_class_wide.c_str();	// pointer to class name
	wc.cbSize = sizeof(WNDCLASSEX);						// strict size for cb size
	RegisterClassEx(&wc);								// register the class
}

HWND RenderWindow::GetHWND() const
{
	return this->handle;
}