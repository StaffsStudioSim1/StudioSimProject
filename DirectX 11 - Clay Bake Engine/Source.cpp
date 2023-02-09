//#include <Windows.h>
#include "ClayEngine.h"
#include "nlohmann/json.hpp"
#include <fstream>
using nlohmann::json;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	Vector2 resolution = { 1296, 720 };
#if !EDIT_MODE
	std::ifstream file("Resources/Settings.json");
	if (!file.good())
		ErrorLogger::Log("Unable to find settings file!");

	json data = json::parse(file);
	resolution = Vector2(data["Resolution"].at(0), data["Resolution"].at(1)); // Get window resolution from the settings file
	file.close();
#endif

	ClayEngine engine;
	if (engine.Initialize(hInstance, "ClayBakeEngine: The Two Magneteers", "ClayBakeEngine", (UINT)resolution.x, (UINT)resolution.y))
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