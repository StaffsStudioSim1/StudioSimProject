#include"WindowContainer.h"

WindowContainer::WindowContainer()
{
	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01; // mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0; 
		rid.hwndTarget = NULL;
		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			ErrorLogger::Log(GetLastError(), "failed to register raw input devices.");
			exit(-1);
		}
		raw_input_initialized = true;
	}
}

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
