//#include <Windows.h>
#include "ClayEngine.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	ClayEngine engine;
	if (engine.Initialize(hInstance, "title", "myWindowClass", 1280, 720))
	{
		while (engine.ProcessMessages() == true)
		{
			engine.Update();
			engine.RenderFrame();
		}
	}
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

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