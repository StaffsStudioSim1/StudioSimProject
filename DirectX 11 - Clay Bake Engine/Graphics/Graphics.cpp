#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	if (!InitializeDirectX(hwnd, width, height))
	{
		return false;
	}
	if (!InitializeShaders())
	{
		return false;
	}
	// place to init scene's ect 
	return true;
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		ErrorLogger::Log("No DXGI Adapters found");
	}
	else if (adapters.size() >= 1)
	{	
		if (adapters.size() >= 2)
		{
			// use this to add a thing to chose gpu's over cpu virtal gpu if available otherwise is set to default 1st gpu available 
		}else
		{ 
			// this will be for if only 1 renderer is availbe if toggle stuff is set up
		}

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
		
		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 2;

		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE; 
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr;

		hr =D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, // IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,							// Graphics device
			NULL,												// software driver type 
			NULL,												// feature lvls array
			NULL,												// flags for^runtime layers 
			0,													// num feature levels in array
			D3D11_SDK_VERSION,									// direct 3d sdk ver
			&scd,												// swap chain desc
			this->swapChain.GetAddressOf(),						// swap-chain ref 
			this->device.GetAddressOf(),						// device ref
			NULL,												// supported feature lvl
			this->deviceContext.GetAddressOf()					// device context	
			);				
		
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "Failed to Create device and swap-chain.\n");
			return false;
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

		hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**> (backBuffer.GetAddressOf()));
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "GetBuffer Failed\n");
			return false;
		}
		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargertView.GetAddressOf());
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "Failed to create render target view\n");
			return false;
		}
		this->deviceContext->OMSetRenderTargets(1,this->renderTargertView.GetAddressOf(), NULL); // null value is render stencil view to be setup 
			
		//create viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = width;
		viewport.Height = height;

		//set viewport
		this->deviceContext->RSSetViewports(1, &viewport); // can add additional view-ports via this 
		
		return true;
	}															
	return false;
//	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL);
	
}

bool Graphics::InitializeShaders()
{

	std::wstring shaderfolder = L"";
	#pragma region DetermineShaderPath
		if (IsDebuggerPresent() == TRUE)
		{
			#ifdef _DEBUG //Debug Mode
			#ifdef _WIN64 //x64
				shaderfolder = L"..\\x64\\Debug\\";
			#else  //x86 (Win32)
				shaderfolder = L"..\\Debug\\";
			#endif
			#else //Release Mode
			#ifdef _WIN64 //x64
				shaderfolder = L"..\\x64\\Release\\";
			#else  //x86 (Win32)
				shaderfolder = L"..\\Release\\";
			#endif
			#endif
	}


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexshader.Initialize(this->device, shaderfolder+ L"vertexshader.cso", layout, numElements))
	{
		return false;
	}
	if (!pixelshader.Initialize(this->device, shaderfolder + L"pixelshader.cso"))
	{
		return false;
	}

	
	return true;
}

bool Graphics::InitializeScene()
{
	Vertex v[] =
	{
		Vertex(0.0f, -0.11f), // center
		Vertex(-0.11f, 0.0f), // left
		Vertex(0.11f, 0.0f), // right
		Vertex(0.0f, 0.11f), // top

	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;

	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}

		return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = {1.0f, 0.0f, 1.0f, 1.0f};
	//this->deviceContext->OMSetRenderTargets(1, this->renderTargertView.GetAddressOf(), NULL);
	this->deviceContext->ClearRenderTargetView(this->renderTargertView.Get(), bgcolor);
	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	this->deviceContext->Draw(3, 0);

	this->swapChain->Present(1, NULL); // FIRST VALUE 1 = VSYNC ON | 0 = VYSNC OFF 
}

