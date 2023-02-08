//#include <Windows.h>
#include "ClayEngine.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	ClayEngine engine;
	if (engine.Initialize(hInstance, "ClayEngine", "myWindowClass", 648 * 2, 360 * 2))
	{
		while (engine.ProcessMessages() == true)
		{
			engine.Update();
			engine.RenderFrame();
		}
	}
	engine.Destroy();

	/*HRESULT hr = S_OK;

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Test Error"); // EXAMPLE ERROR CHECK
	}
	if (SUCCEEDED(hr))
	{
		MessageBoxA(NULL, "success", "Yay", NULL);
	}*/

	return 0;
}