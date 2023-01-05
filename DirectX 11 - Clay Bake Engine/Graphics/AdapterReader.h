#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXtk.lib")
#pragma comment(lib, "DXGI.lib")
#include"..\\ErrorLogger.h"
#include<d3d11.h>
#include<wrl/client.h>
#include<vector>

class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter;
	DXGI_ADAPTER_DESC description;

};

class AdapterReader
{
public:
	static std::vector<AdapterData> GetAdapters();
protected:

private:
	static std::vector<AdapterData> adapters;
};