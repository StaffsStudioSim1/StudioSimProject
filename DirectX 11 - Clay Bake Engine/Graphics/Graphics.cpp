#include "Graphics.h"
#include "../GameObjects/ObjectHandler.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "../SceneManager.h"

#include "../GameObjects/Components/ButtonComponent.h"
#include "../GameObjects/Components/LeverComponent.h"
#include "../GameObjects/Components/PressurePlateComponent.h"
#include "../GameObjects/Components/Goal.h"

#include <fstream>
#include "../Audio/AudioManager.h"

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
	InitializeScene();

	// World matrix
	DirectX::XMStoreFloat4x4(&_world, DirectX::XMMatrixIdentity());

	// View matrix
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&_view, DirectX::XMMatrixLookAtLH(eye, at, up));

	// Projection matrix
	DirectX::XMStoreFloat4x4(&_projection, DirectX::XMMatrixOrthographicLH(INTERNAL_RESOLUTION_X, INTERNAL_RESOLUTION_Y, 0.01f, 100.0f));

	//IMGUI SETUP
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->_device.Get(), this->_deviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
	_windowWidth = width;
	_windowHeight = height;

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
		}

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		HRESULT hr;

		bool isWindowed = true;
#if !EDIT_MODE
		std::ifstream file("Resources/Settings.json");
		if (!file.good())
			ErrorLogger::Log("Unable to find settings file!");

		json data = json::parse(file);
		isWindowed = !data["Fullscreen"];
		file.close();
#endif
		
		if (_initialStart)
		{
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
			scd.Windowed = isWindowed;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, // IDXGI Adapter
				D3D_DRIVER_TYPE_UNKNOWN,							// Graphics device
				NULL,												// software driver type 
				D3D11_CREATE_DEVICE_DEBUG,							// feature lvls array
				NULL,												// flags for^runtime layers 
				0,													// num feature levels in array
				D3D11_SDK_VERSION,									// direct 3d sdk ver
				&scd,												// swap chain desc
				this->_swapChain.GetAddressOf(),						// swap-chain ref 
				this->_device.GetAddressOf(),						// device ref
				NULL,												// supported feature lvl
				this->_deviceContext.GetAddressOf()					// device context	
			);

			if (FAILED(hr))
				ErrorLogger::Log(hr, "Failed to Create device and swap-chain.\n");
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

		hr = this->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**> (backBuffer.GetAddressOf()));
		if (FAILED(hr))
			ErrorLogger::Log(hr, "GetBuffer Failed\n");

		hr = this->_device->CreateRenderTargetView(backBuffer.Get(), NULL, this->_renderTargertView.GetAddressOf());
		if (FAILED(hr))
			ErrorLogger::Log(hr, "Failed to create render target view\n");

		hr = this->_device->CreateTexture2D(&depthStencilDesc, nullptr, _depthStencilBuffer.GetAddressOf());
		if (FAILED(hr))
			ErrorLogger::Log(hr, "Failed to create Texture2D\n");

		hr = this->_device->CreateDepthStencilView(_depthStencilBuffer.Get(), nullptr, _depthStencilView.GetAddressOf());
		if (FAILED(hr))
			ErrorLogger::Log(hr, "Failed to create depth stencil view\n");

		this->_deviceContext->OMSetRenderTargets(1, this->_renderTargertView.GetAddressOf(), _depthStencilView.Get());

		if (_initialStart)
		{
			//create viewport
			D3D11_VIEWPORT viewport;
			ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = width;
			viewport.Height = height;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;

			//set viewport
			this->_deviceContext->RSSetViewports(1, &viewport); // can add additional view-ports via this
			_initialStart = false;
		}
		else
		{
			D3D11_VIEWPORT viewport;
			UINT vpNum = 1;
			this->_deviceContext->RSGetViewports(&vpNum, &viewport);
			viewport.Width = width;
			viewport.Height = height;
			this->_deviceContext->RSSetViewports(1, &viewport);
		}

		// Create stencil state
		D3D11_DEPTH_STENCIL_DESC stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		stencilDesc.StencilEnable = false;

		hr = this->_device->CreateDepthStencilState(&stencilDesc, _stencilState.GetAddressOf());
		if (FAILED(hr))
			ErrorLogger::Log(hr, "Failed to create depth stencil state\n");

		this->_deviceContext->OMSetDepthStencilState(_stencilState.Get(), 1);

		// Create sampler state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = this->_device->CreateSamplerState(&sampDesc, _samplerState.GetAddressOf());
		if (FAILED(hr))
			ErrorLogger::Log(hr, "Failed to create sampler state\n");

		this->_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		// Create rasteriser states - wireframe and solid
		D3D11_RASTERIZER_DESC rasterDesc0 = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		ZeroMemory(&rasterDesc0, sizeof(D3D11_RASTERIZER_DESC));
		rasterDesc0.MultisampleEnable = true;
		rasterDesc0.AntialiasedLineEnable = true;
		rasterDesc0.FillMode = D3D11_FILL_WIREFRAME;
		rasterDesc0.CullMode = D3D11_CULL_NONE;
		hr = this->_device->CreateRasterizerState(&rasterDesc0, _wireframeRasterState.GetAddressOf());
		if (FAILED(hr))
			ErrorLogger::Log(hr, "Failed to create wireframe rasteriser state\n");

		D3D11_RASTERIZER_DESC rasterDesc1 = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		ZeroMemory(&rasterDesc1, sizeof(D3D11_RASTERIZER_DESC));
		rasterDesc1.MultisampleEnable = true;
		rasterDesc1.AntialiasedLineEnable = true;
		rasterDesc1.FillMode = D3D11_FILL_SOLID;
		rasterDesc1.CullMode = D3D11_CULL_NONE; // Leave as none due to the way we're using scale
		hr = this->_device->CreateRasterizerState(&rasterDesc1, _solidRasterState.GetAddressOf());
		if (FAILED(hr))
			ErrorLogger::Log(hr, "Failed to create solid rasteriser state\n");

		this->_deviceContext->RSSetState(_solidRasterState.Get());

		// Create blender state
		D3D11_RENDER_TARGET_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.BlendEnable = true;
		blendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.SrcBlend = D3D11_BLEND_ONE;
		blendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;

		D3D11_BLEND_DESC bDesc = { 0 };
		bDesc.RenderTarget[0] = blendDesc;
		hr = this->_device->CreateBlendState(&bDesc, _blendState.GetAddressOf());
		if (FAILED(hr))
			ErrorLogger::Log(hr, "Failed to create blender state\n");

		this->_deviceContext->OMSetBlendState(_blendState.Get(), NULL, 0xFFFFFFFF);

		ObjectHandler::GetInstance().Initialise(_device);

		return true;
	}
	return false;
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
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!_vertexshader.Initialize(this->_device, shaderfolder + L"vertexshader.cso", layout, numElements))
	{
		return false;
	}
	if (!_pixelshader.Initialize(this->_device, shaderfolder + L"pixelshader.cso"))
	{
		return false;
	}

	this->_deviceContext->IASetInputLayout(this->_vertexshader.GetInputLayout());

	return true;
	}

bool Graphics::InitializeScene()
{
	// Create a primitive square
	SimpleVertex v[] =
	{
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex) * ARRAYSIZE(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;

	HRESULT hr = this->_device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->_vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}

	WORD indices[] =
	{
		3, 0, 1,
		2, 0, 3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * ARRAYSIZE(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = indices;
	hr = this->_device->CreateBuffer(&indexBufferDesc, &indexBufferData, this->_indexBuffer.GetAddressOf());
	if (FAILED(hr))
		ErrorLogger::Log(hr, "Failed to create index buffer\n");

	this->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = this->_device->CreateBuffer(&bd, nullptr, _constantBuffer.GetAddressOf());
	if (FAILED(hr))
		ErrorLogger::Log(hr, "Failed to create constant buffer\n");

	// Save the square shape data
	ObjectHandler::GetInstance().SetSquareGeometry(this->_vertexBuffer, this->_indexBuffer, ARRAYSIZE(indices), 0, sizeof(SimpleVertex));

	return true;
}

void Graphics::ResizeWindow()
{
	// Update window size
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(MonitorFromWindow(GetActiveWindow(), MONITOR_DEFAULTTONEAREST), &mi);
	UINT monitorX = mi.rcMonitor.right - mi.rcMonitor.left;
	UINT monitorY = mi.rcMonitor.bottom - mi.rcMonitor.top;

	int centreOfScreenX = mi.rcMonitor.left + (monitorX / 2 - _resolutionWidth / 2);
	int centreOfScreenY = mi.rcMonitor.top + (monitorY / 2 - _resolutionHeight / 2);

	RECT rc = { (LONG)centreOfScreenX, (LONG)centreOfScreenY, (LONG)centreOfScreenX + (LONG)_resolutionWidth, (LONG)centreOfScreenY + (LONG)_resolutionHeight };
	AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	SetWindowPos(GetActiveWindow(), NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	
	POINT pt;
	pt.x = centreOfScreenX;
	pt.y = centreOfScreenY;
	ClientToScreen(GetActiveWindow(), &pt);

	// Destroy and recreate graphics buffers
	_deviceContext->OMSetRenderTargets(0, 0, 0);

	_blendState.Reset();
	_depthStencilBuffer.Reset();
	_depthStencilView.Reset();
	_renderTargertView.Reset();
	_solidRasterState.Reset();
	_wireframeRasterState.Reset();
	_samplerState.Reset();
	_stencilState.Reset();

	_deviceContext->Flush();

	_swapChain->ResizeBuffers(0, _resolutionWidth, _resolutionHeight, DXGI_FORMAT_UNKNOWN, 0);
	_swapChain->SetFullscreenState(_useFullscreen, NULL);

	InitializeDirectX(GetActiveWindow(), _resolutionWidth, _resolutionHeight);
}

void Graphics::LoadSettingsFromFile()
{
	std::ifstream inFile("Resources/Settings.json");
	if (!inFile.good())
		ErrorLogger::Log("Unable to retrieve settings from file!");

	json data = json::parse(inFile);
	_resolutionWidth = data["Resolution"].at(0);
	_resolutionHeight = data["Resolution"].at(1);
	_useFullscreen = data["Fullscreen"];
	//_musicVol = data["MusicVol"];
	_soundVol = data["SoundVol"];
	inFile.close();

	switch (_resolutionWidth)
	{
	case 1280:
	{
		_currentResolution = 0;
		break;
	}
	case 1600:
	{
		_currentResolution = 1;
		break;
	}
	case 1920:
	{
		_currentResolution = 2;
		break;
	}
	default:
		_currentResolution = 0;
		break;
	}
}

void Graphics::RenderFrame(Scene* scene)
{
	float bgcolor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	this->_deviceContext->ClearRenderTargetView(this->_renderTargertView.Get(), bgcolor);
	this->_deviceContext->ClearDepthStencilView(this->_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	DirectX::XMMATRIX worldMatrix = XMLoadFloat4x4(&_world);
	DirectX::XMMATRIX viewMatrix = XMLoadFloat4x4(&_view);
	DirectX::XMMATRIX projectionMatrix = XMLoadFloat4x4(&_projection);

	ConstantBuffer cb;
	cb.mView = DirectX::XMMatrixTranspose(viewMatrix);
	cb.mProjection = DirectX::XMMatrixTranspose(projectionMatrix);

	this->_deviceContext->VSSetShader(_vertexshader.GetShader(), NULL, 0);
	this->_deviceContext->PSSetShader(_pixelshader.GetShader(), NULL, 0);

	this->_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	this->_deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

	scene->Render(this->_deviceContext, cb, _constantBuffer);

	//UI
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	//CREATE FRAME
	ImGui::NewFrame();
	//UI WINDOWS
	if (SceneManager::GetInstance().GetCurrentSceneID() == 0 && ObjectHandler::GetInstance().IsMainMainUIEnabled()) // Main menu UI
	{
		const char* playButton = "Resources/Sprites/PlayButton.dds";
		const char* optionsButton = "Resources/Sprites/OptionsButton.dds";
		const char* levelSelect = "Resources/Sprites/LevelSelect.dds";
		const char* exitButton = "Resources/Sprites/ExitButton.dds";

		TextureInfo playButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(playButton);
		TextureInfo optionsButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(optionsButton);
		TextureInfo levelSelectText = ObjectHandler::GetInstance().LoadDDSTextureFile(levelSelect);
		TextureInfo exitButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(exitButton);


		ImVec2 size = ImVec2(playButtonText.width * 2 * (float)(_windowWidth / 1280.0f), playButtonText.height * 2 * (float)(_windowHeight / 720.0f));

		ImGui::SetNextWindowSize({ (float)_windowWidth, (float)_windowHeight});
		ImGui::SetNextWindowPos({ (float)(_windowWidth / 2) - (size.x / 2), (float)(_windowHeight / 2) - (size.y * 3) });
		ImGui::Begin("Menu", NULL, window_flags);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.75f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.55f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.45f));
		if (ImGui::ImageButton(playButton, playButtonText.texture, size))
		{
			SceneManager::GetInstance().LoadScene("Resources/demo.json");
		}
		if (ImGui::ImageButton(levelSelect, levelSelectText.texture, size))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(true);
			ObjectHandler::GetInstance().EnableMainMenuUI(false);
		}
		if (ImGui::ImageButton(optionsButton, optionsButtonText.texture, size))
		{
			LoadSettingsFromFile();
			ObjectHandler::GetInstance().EnableOptionsMenuUI(true);
			ObjectHandler::GetInstance().EnableMainMenuUI(false);
		}
		if (ImGui::ImageButton(exitButton, exitButtonText.texture, size))
		{
			exit(0);
		}
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	if (ObjectHandler::GetInstance().IsLevelSelectUIEnabled()) // Level select UI
	{
		const char* level1Button = "Resources/Sprites/Level1Button.dds";
		const char* level2Button = "Resources/Sprites/Level2Button.dds";
		const char* level3Button = "Resources/Sprites/Level3Button.dds";
		const char* level4Button = "Resources/Sprites/Level4Button.dds";
		const char* level5Button = "Resources/Sprites/Level5Button.dds";
		const char* level6Button = "Resources/Sprites/Level6Button.dds";
		const char* level7Button = "Resources/Sprites/Level7Button.dds";
		const char* backButton = "Resources/Sprites/BackIcon.dds";


		TextureInfo level1ButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(level1Button);
		TextureInfo level2ButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(level2Button);
		TextureInfo level3ButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(level3Button);
		TextureInfo level4ButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(level4Button);
		TextureInfo level5ButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(level5Button);
		TextureInfo level6ButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(level6Button);
		TextureInfo level7ButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(level7Button);
		TextureInfo backButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(backButton);


		ImVec2 size = ImVec2(level1ButtonText.width * 2 * (float)(_windowWidth / 1280.0f), level1ButtonText.height * 2 * (float)(_windowHeight / 720.0f));

		ImGui::SetNextWindowSize({ (float)_windowWidth, (float)_windowHeight });
		ImGui::SetNextWindowPos({ (float)(_windowWidth / 2) - (size.x / 2), (float)(_windowHeight / 2) - (size.y * 3) });
		ImGui::Begin("Level Select", NULL, window_flags);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.75f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.55f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.45f));
		if (ImGui::ImageButton(level1Button, level1ButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			SceneManager::GetInstance().LoadScene("Resources/demo.json");
		}
		if (ImGui::ImageButton(level2Button, level2ButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			SceneManager::GetInstance().LoadScene("Resources/demo.json");
		}
		if (ImGui::ImageButton(level3Button, level3ButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			SceneManager::GetInstance().LoadScene("Resources/demo.json");
		}
		if (ImGui::ImageButton(level4Button, level4ButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			SceneManager::GetInstance().LoadScene("Resources/demo.json");
		}
		if (ImGui::ImageButton(level5Button, level5ButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			SceneManager::GetInstance().LoadScene("Resources/demo.json");
		}
		if (ImGui::ImageButton(level6Button, level6ButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			SceneManager::GetInstance().LoadScene("Resources/demo.json");
		}
		if (ImGui::ImageButton(level7Button, level7ButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			SceneManager::GetInstance().LoadScene("Resources/demo.json");
		}
		if (ImGui::ImageButton(backButton, backButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			ObjectHandler::GetInstance().EnableMainMenuUI(true);
		}

		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	if (ObjectHandler::GetInstance().IsPauseMenuUIEnabled()) // Pause menu UI
	{
		const char* resumeButton = "Resources/Sprites/ResumeButton.dds";
		const char* resetButton = "Resources/Sprites/ResetButton.dds";
		const char* pOptionsButton = "Resources/Sprites/PauseOptionsButton.dds";
		const char* mainMenuButton = "Resources/Sprites/MainMenuButton.dds";
		const char* exitGameButton = "Resources/Sprites/ExitGameButton.dds";
		const char* pauseMenu = "Resources/Sprites/PauseMenuBackground.dds";

		TextureInfo resumeButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(resumeButton);
		TextureInfo resetButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(resetButton);
		TextureInfo pOptionsButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(pOptionsButton);
		TextureInfo menuButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(mainMenuButton);
		TextureInfo exitGameButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(exitGameButton);
		TextureInfo pauseMenuText = ObjectHandler::GetInstance().LoadDDSTextureFile(pauseMenu);

		ImVec2 size = ImVec2(resumeButtonText.width * 1.5 * (float)(_windowWidth / 1280.0f), resumeButtonText.height * 1.5 * (float)(_windowHeight / 720.0f));
		ImVec2 sizeP = ImVec2(pauseMenuText.width * 1.5 * (float)(_windowWidth / 1280.0f), pauseMenuText.height * 1.5 * (float)(_windowHeight / 720.0f));


		ImGui::SetNextWindowSize({ (float)_windowWidth, (float)_windowHeight });
		ImGui::SetNextWindowPos({ (float)(_windowWidth / 2) - (sizeP.x / 2), (float)(_windowHeight / 2) - (sizeP.y / 2)});
		ImGui::Begin("PauseMenuBG", NULL, window_flags | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::Image(pauseMenuText.texture, sizeP);
		ImGui::End();

		ImGui::SetNextWindowSize({ (float)_windowWidth, (float)_windowHeight });
		ImGui::SetNextWindowPos({ (float)(_windowWidth / 2) - (size.x / 2), (float)(_windowHeight / 2) - (size.y * 3) });
		ImGui::Begin("PauseMenu", NULL, window_flags);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.75f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.55f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.45f));
		if (ImGui::ImageButton(resumeButton, resumeButtonText.texture, size))
		{
			GameManager::GetInstance().Pause();
			// Code to unpause of the game here or somewhere else
		}
		if (ImGui::ImageButton(resetButton, resetButtonText.texture, size))
		{
			GameManager::GetInstance().Pause();
			SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetCurrentSceneFilePath());
		}
		if (ImGui::ImageButton(pOptionsButton, pOptionsButtonText.texture, size))
		{
			LoadSettingsFromFile();
			ObjectHandler::GetInstance().EnableOptionsMenuUI(true);
			ObjectHandler::GetInstance().EnablePauseMenuUI(false);
		}
		if (ImGui::ImageButton(mainMenuButton, menuButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnablePauseMenuUI(false);
			SceneManager::GetInstance().LoadScene("Resources/MainMenu.json");
		}
		if (ImGui::ImageButton(exitGameButton, exitGameButtonText.texture, size))
		{
			exit(0);
		}
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	if (ObjectHandler::GetInstance().IsOptionsMenuUIEnabled()) // Options UI
	{
		const char* applyButton = "Resources/Sprites/ApplyIcon.dds";
		const char* backButton = "Resources/Sprites/BackIcon.dds";
		const char* leftButton = "Resources/Sprites/LeftIcon.dds";
		const char* rightButton = "Resources/Sprites/RightIcon.dds";
		const char* fullscreenIcon = "Resources/Sprites/FullScreenIcon.dds";
		const char* res720Icon = "Resources/Sprites/720pIcon.dds";
		const char* res900Icon = "Resources/Sprites/900pIcon.dds";
		const char* res1080Icon = "Resources/Sprites/1080pIcon.dds";
		const char* soundIcon = "Resources/Sprites/SoundIcon.dds";
		const char* musicIcon = "Resources/Sprites/MusicIcon.dds";
		const char* opionsMenu = "Resources/Sprites/OptionsBackground.dds";



		TextureInfo applyButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(applyButton);
		TextureInfo backButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(backButton);
		TextureInfo leftButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(leftButton);
		TextureInfo rightButtonText = ObjectHandler::GetInstance().LoadDDSTextureFile(rightButton);
		TextureInfo fullscreenIconText = ObjectHandler::GetInstance().LoadDDSTextureFile(fullscreenIcon);
		TextureInfo res720IconText = ObjectHandler::GetInstance().LoadDDSTextureFile(res720Icon);
		TextureInfo res900IconText = ObjectHandler::GetInstance().LoadDDSTextureFile(res900Icon);
		TextureInfo res1080IconText = ObjectHandler::GetInstance().LoadDDSTextureFile(res1080Icon);
		TextureInfo soundIconText = ObjectHandler::GetInstance().LoadDDSTextureFile(soundIcon);
		TextureInfo musicIconText = ObjectHandler::GetInstance().LoadDDSTextureFile(musicIcon);
		TextureInfo optionsMenuText = ObjectHandler::GetInstance().LoadDDSTextureFile(opionsMenu);


		ImVec2 size = ImVec2(applyButtonText.width * (float)(_windowWidth / 1280.0f), applyButtonText.height * (float)(_windowHeight / 720.0f));
		ImVec2 sizeFS = ImVec2(fullscreenIconText.width * (float)(_windowWidth / 1280.0f), fullscreenIconText.height * (float)(_windowHeight / 720.0f));
		ImVec2 sizeLR = ImVec2(leftButtonText.width * (float)(_windowWidth / 1280.0f), leftButtonText.height * (float)(_windowHeight / 720.0f));
		ImVec2 sizeO = ImVec2(optionsMenuText.width * 1.5 * (float)(_windowWidth / 1280.0f), optionsMenuText.height * 1.5 * (float)(_windowHeight / 720.0f));


		std::vector<TextureInfo> resolutionText = { res720IconText,res900IconText,res1080IconText };

		ImGui::SetNextWindowSize({ (float)_windowWidth, (float)_windowHeight });
		ImGui::SetNextWindowPos({ (float)(_windowWidth / 2) - (sizeO.x / 2), (float)(_windowHeight / 2) - (sizeO.y / 2) });
		ImGui::Begin("OptionsBG", NULL, window_flags | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::Image(optionsMenuText.texture, sizeO);
		ImGui::End();

		ImGui::SetNextWindowSize({ (float)_windowWidth, (float)_windowHeight });
		ImGui::SetNextWindowPos({ (float)(_windowWidth / 2) - (sizeFS.x * 1.5f), (float)(_windowHeight / 2) - (sizeFS.y * 4) });
		ImGui::Begin("Options Menu", NULL, window_flags);

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.75f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.55f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.06f, 0.45f));

		if (ImGui::ImageButton(leftButton, leftButtonText.texture, sizeLR))
		{
			if (_currentResolution > 0)
				_currentResolution -= 1;
		}
		ImGui::SameLine();
		ImGui::Image(resolutionText[_currentResolution].texture, sizeFS);
		ImGui::SameLine();
		if (ImGui::ImageButton(rightButton, rightButtonText.texture, sizeLR))
		{
			if (_currentResolution < resolutionText.size()-1)
				_currentResolution += 1;
		}
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
#if EDIT_MODE
		ImGui::SetTooltip("Resolution and fullscreen settings are disabled in edit mode");
#endif
		ImGui::Image(fullscreenIconText.texture, sizeFS);
		ImGui::SameLine();
		ImGui::Checkbox("  ", &_useFullscreen);

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		//ImGui::SliderInt("  ", &_musicVol, 0, 100);
		//ImGui::SameLine();
		//ImGui::Image(musicIconText.texture, size);

		ImGui::PushItemWidth(250);
		ImGui::Image(soundIconText.texture, size);
		ImGui::SameLine();
		ImGui::SliderInt(" ", &_soundVol, 0, 100);

		ImGui::Dummy(ImVec2(0.0f, 5.0f));


		if (ImGui::ImageButton(applyButton, applyButtonText.texture, size))
		{
			// Apply setting changes
			switch (_currentResolution)
			{
			case 0:
			{
				_resolutionWidth = 1280;
				_resolutionHeight = 720;
				break;
			}
			case 1:
			{
				_resolutionWidth = 1600;
				_resolutionHeight = 900;
				break;
			}
			case 2:
			{
				_resolutionWidth = 1920;
				_resolutionHeight = 1080;
				break;
			}
			default:
				_resolutionWidth = 1280;
				_resolutionHeight = 720;
				break;
			}

			MONITORINFO mi = { sizeof(mi) };
			GetMonitorInfo(MonitorFromWindow(GetActiveWindow(), MONITOR_DEFAULTTONEAREST), &mi);
			UINT monitorX = mi.rcMonitor.right - mi.rcMonitor.left;
			UINT monitorY = mi.rcMonitor.bottom - mi.rcMonitor.top;

			if (_useFullscreen && (_resolutionWidth != monitorX || _resolutionHeight != monitorY))
			{
				_resolutionWidth = monitorX;
				_resolutionHeight = monitorY;
			}

			AudioManager::GetInstance().SetMasterVolume(_soundVol);

			json settings;
			settings["Resolution"] = { _resolutionWidth, _resolutionHeight };
			settings["Fullscreen"] = _useFullscreen;
			//settings["MusicVol"] = _musicVol;
			settings["SoundVol"] = _soundVol;

			std::ofstream outFile("Resources/Settings.json");
			outFile << std::setw(4) << settings << std::endl;
			outFile.close();

#if !EDIT_MODE
			ResizeWindow();
			ResizeWindow();
#endif
		}
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		if (ImGui::ImageButton(backButton, backButtonText.texture, size))
		{
			ObjectHandler::GetInstance().EnableOptionsMenuUI(false);
			if (SceneManager::GetInstance().GetCurrentSceneID() == 0)
				ObjectHandler::GetInstance().EnableMainMenuUI(true);
			else
				ObjectHandler::GetInstance().EnablePauseMenuUI(true);
		}
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
#if EDIT_MODE
	static bool linkScaling = true;
	char fileName[40]; // For saving the file
	strcpy_s(fileName, scene->GetFilePath().c_str());

	const char* interactableStates[] = { "Default", "SwitchState", "SwitchGravity", "Signal3", "Signal4" };
	
	ImGui::Begin("Inspector");
	if (SceneManager::GetInstance().GetCurrentSceneID() != 0)
	{
		if (ImGui::Button("Pause Toggle"))
		{
			ObjectHandler::GetInstance().EnablePauseMenuUI(!ObjectHandler::GetInstance().IsPauseMenuUIEnabled());
		}
	}
	if (ImGui::TreeNode("Game Objects"))
	{
		int loopNum = 0;
		for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
		{
			if (object->GetTag() != JSON_TAG_GAMEOBJECT)
				continue;
			// Use SetNextItemOpen() so set the default state of a node to be open. We could
			// also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
			if (loopNum == 0)
				ImGui::SetNextItemOpen(false, ImGuiCond_Once);

			if (ImGui::TreeNode(object->GetName().c_str()))
			{
				std::string name = object->GetName();
				char nameChar[40];
				strcpy_s(nameChar, name.c_str());

				float position[2] = { object->GetTransform()->GetPosition().x, object->GetTransform()->GetPosition().y };
				float depth = { object->GetTransform()->GetDepthPos() };
				float rotation = { object->GetTransform()->GetRotation() };
				float scale[2] = { object->GetTransform()->GetScale().x, object->GetTransform()->GetScale().y };

				bool hasAppearance = false;

				bool hasButton = false;
				bool hasLever = false;
				bool hasPressurePlate = false;
				bool hasGoal = false;

				std::string textureName;
				char textureNameChar[40];
				float texCoords[4] = { 0 };
				if (object->GetComponent<Appearance>())
				{
					hasAppearance = true;
					textureName = object->GetComponent<Appearance>()->GetTexture().filePath;
					strcpy_s(textureNameChar, textureName.c_str());
					DirectX::XMFLOAT4 coords = object->GetComponent<Appearance>()->GetTexCoordFrameValues();
					texCoords[0] = coords.x;
					texCoords[1] = coords.y;
					texCoords[2] = coords.z;
					texCoords[3] = coords.w;
				}

				int switchState = 0;
				std::string linkedObject;
				char linkedObjectChar[40];
				if (object->GetComponent<ButtonComponent>())
				{
					hasButton = true;
					linkedObject = object->GetComponent<Interactable>()->_linkedObjectName;
					strcpy_s(linkedObjectChar, linkedObject.c_str());
				}
				else if (object->GetComponent<LeverComponent>())
				{
					hasLever = true;
					linkedObject = object->GetComponent<Interactable>()->_linkedObjectName;
					strcpy_s(linkedObjectChar, linkedObject.c_str());
				}
				else if (object->GetComponent<PressurePlateComponent>())
				{
					hasPressurePlate = true;
					linkedObject = object->GetComponent<Interactable>()->_linkedObjectName;
					strcpy_s(linkedObjectChar, linkedObject.c_str());
				}
				else if (object->GetComponent<Goal>())
				{
					hasGoal = true;
					linkedObject = object->GetComponent<Interactable>()->_linkedObjectName;
					strcpy_s(linkedObjectChar, linkedObject.c_str()); // Uses linked object string for next level name
				}

				int bodyType;
				float density, friction;

				ImGui::PushItemWidth(250); // Sets the pixel width of the input boxes

				if (ImGui::InputText("Name", nameChar, 40, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					object->SetName(nameChar);
				}
				ImGui::DragFloat("X Position", &position[0], 18.0f, -315.0f, 315.0f);
				ImGui::DragFloat("Y Position", &position[1], 18.0f, -171.0f, 171.0f);
				ImGui::DragFloat("Depth", &depth, 0.005f, 0.0f, 1.0f);
				ImGui::DragFloat("Rotation", &rotation, 0.025f, 0.0f, DirectX::XM_2PI);
				ImGui::DragFloat2("Scale", scale, 0.05f, -100, 100);
				ImGui::SameLine();
				ImGui::Checkbox("Link scaling", &linkScaling);
				if (hasAppearance)
				{
					if (ImGui::InputText("Texture Name", textureNameChar, 40, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						TextureInfo newTexture = ObjectHandler::GetInstance().LoadDDSTextureFile(textureNameChar, true);
						if (newTexture.filePath != "") // Prevents a potential crash
							object->GetComponent<Appearance>()->SetTexture(newTexture);
					}
					ImGui::DragFloat4("Texture Coords", texCoords, 1.0f, 0.0f, 10.0f);
				}
				if (hasButton || hasLever || hasPressurePlate)
				{
					if (ImGui::InputText("Linked Object", linkedObjectChar, 40, ImGuiInputTextFlags_EnterReturnsTrue))
					switchState = object->GetComponent<Interactable>()->interactableLink;
					ImGui::ListBox("Switch State", &switchState, interactableStates, 5);
				}
				if (hasGoal)
				{
					ImGui::InputText("Next Level File Path", linkedObjectChar, 40);
				}
				if (ImGui::Button("Reset"))
				{
					depth = 0.0f;
					scale[0] = 1.0f;
					scale[1] = 1.0f;
					rotation = 0.0f;
				}
				ImGui::TreePop();

				object->GetTransform()->SetPosition(position[0], position[1]);
				object->GetTransform()->SetDepthPos(depth);
				if (linkScaling)
					scale[1] = scale[0];
				object->GetTransform()->SetScale(scale[0], scale[1]);
				object->GetTransform()->SetRotation(rotation);
				if (hasAppearance)
				{
					object->GetComponent<Appearance>()->SetTexCoords(texCoords[0], texCoords[1], texCoords[2], texCoords[3]);
				}
				if (hasButton || hasLever || hasPressurePlate)
				{
					object->GetComponent<Interactable>()->_linkedObjectName = linkedObjectChar;
					object->GetComponent<Interactable>()->interactableLink = (Interactable::InteractableLink)switchState;
				}
				if (hasGoal)
				{
					object->GetComponent<Goal>()->_NextLevelName = linkedObjectChar;
				}
			}
			loopNum++;
		}
		ImGui::TreePop();
	}
	ImGui::PushItemWidth(200);

	if(ImGui::InputText("File Name", fileName, 40))
		scene->SetFileName(fileName);


	ImGui::SameLine();
	if (ImGui::Button("Save"))
		scene->Save();

	if (ImGui::Button("Load"))
	{
		std::string sFileName = fileName;
		

		SceneManager::GetInstance().LoadScene(fileName);
	}

	ImGui::SameLine();
	if (ImGui::Button("Create New Scene"))
		scene->Save();

	ImGui::End();
#endif
	// ASSEMBLE AND RENDER DRAW DATA
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	this->_swapChain->Present(1, NULL); // FIRST VALUE 1 = VSYNC ON 0 = VYSNC OFF 
}

