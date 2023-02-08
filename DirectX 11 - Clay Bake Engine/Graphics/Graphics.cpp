#include "Graphics.h"
#include "../GameObjects/ObjectHandler.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "../SceneManager.h"

#include <fstream>

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

		bool isWindowed = true;
#if !EDIT_MODE
		std::ifstream file("Resources/Settings.json");
		if (!file.good())
			ErrorLogger::Log("Unable to find settings file!");

		json data = json::parse(file);
		isWindowed = !data["Fullscreen"];
		file.close();
#endif

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
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
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
		rasterDesc1.CullMode = D3D11_CULL_BACK;
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
	if (SceneManager::GetInstance().GetCurrentSceneID() == 0 && ObjectHandler::GetInstance().IsMainMainUIEnabled())
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


	if (ObjectHandler::GetInstance().IsLevelSelectUIEnabled())
	{
		ImGui::SetNextWindowSize({ (float)_windowWidth, (float)_windowHeight });
		ImGui::SetNextWindowPos({ (float)(_windowWidth / 2), (float)(_windowHeight / 2) });
		ImGui::Begin("Level Select", NULL, window_flags);
		if (ImGui::Button("Demo"))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			SceneManager::GetInstance().LoadScene("Resources/demo.json");
		}
		if (ImGui::Button("Back"))
		{
			ObjectHandler::GetInstance().EnableLevelSelectUI(false);
			ObjectHandler::GetInstance().EnableMainMenuUI(true);
		}
		ImGui::End();
	}

	if (ObjectHandler::GetInstance().IsOptionsMenuUIEnabled())
	{
		std::vector<std::string> resolution = { "1280x720","1600x900","1920x1080" };
		ImGui::SetNextWindowSize({ (float)_windowWidth, (float)_windowHeight });
		ImGui::SetNextWindowPos({ (float)(_windowWidth / 2), (float)(_windowHeight / 2) });
		ImGui::Begin("Options Menu", NULL, window_flags);
		if (ImGui::ArrowButton("leftArrow", ImGuiDir_Left))
		{
			if (_currentResolution > 0)
				_currentResolution -= 1;
		}
		ImGui::SameLine();
		ImGui::Text(resolution[_currentResolution].c_str());
		ImGui::SameLine();
		if (ImGui::ArrowButton("rightArrow", ImGuiDir_Right))
		{
			if (_currentResolution < resolution.size()-1)
				_currentResolution += 1;
		}
		if (ImGui::Button("Apply"))
		{
			// Apply setting changes
		}
		if (ImGui::Button("Back"))
		{
			ObjectHandler::GetInstance().EnableOptionsMenuUI(false);
			ObjectHandler::GetInstance().EnableMainMenuUI(true);
		}
		ImGui::End();

	}
#if EDIT_MODE
	static bool linkScaling = true;
	char fileName[30]; // For saving the file
	strcpy_s(fileName, scene->GetFilePath().c_str());
	const char* boxBodyChoices[] = { "Static", "Kinematic", "Dynamic" };
	
	ImGui::Begin("Inspector");
	if (ImGui::TreeNode("Game Objects"))
	{
		int loopNum = 0;
		for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
		{
			// Use SetNextItemOpen() so set the default state of a node to be open. We could
			// also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
			if (loopNum == 0)
				ImGui::SetNextItemOpen(false, ImGuiCond_Once);

			if (ImGui::TreeNode(object->GetName().c_str()))
			{
				std::string name = object->GetName();
				char nameChar[20];
				strcpy_s(nameChar, name.c_str());

				float position[2] = { object->GetTransform()->GetPosition().x, object->GetTransform()->GetPosition().y };
				float depth = { object->GetTransform()->GetDepthPos() };
				float rotation = { object->GetTransform()->GetRotation() };
				float scale[2] = { object->GetTransform()->GetScale().x, object->GetTransform()->GetScale().y };

				bool hasPhysics = false;
				bool hasAppearance = false;

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

				int bodyType;
				float density, friction;
				if (object->GetComponent<Physics>())
				{
					hasPhysics = true;
					bodyType = object->GetComponent<Physics>()->GetBodyType();
					density = object->GetComponent<Physics>()->GetDensity();
					friction = object->GetComponent<Physics>()->GetFriction();
				}

				ImGui::PushItemWidth(250); // Sets the pixel width of the input boxes

				if (ImGui::InputText("Name", nameChar, 20, ImGuiInputTextFlags_EnterReturnsTrue))
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
				if (hasPhysics)
				{
					ImGui::ListBox("Body Type", &bodyType, boxBodyChoices, IM_ARRAYSIZE(boxBodyChoices), 3);
					ImGui::DragFloat("Density", &density, 0.025f, 0.0f, 100.0f);
					ImGui::DragFloat("Friction", &friction, 0.0025f, 0.0f, 1.0f);

					if (ImGui::Button("Remove Physics")) // Remove physics from object
					{
						object->RemoveComponent(object->GetComponent<Physics>());
						hasPhysics = false;
					}
				}
				else
				{
					if (ImGui::Button("Add Physics")) // Add physics to object
					{
						Component* component = nullptr;
						PhysicsBody* body = new PhysicsBody();
						body->bodyDef.startPos = object->GetTransform()->GetPosition();
						body->bodyDef.startingRoatation = object->GetTransform()->GetRotation();
						body->hitboxdef.bodyType = Dynmaic;
						body->bodyDef.density = 0.1f;
						body->bodyDef.friction = 1.0f;
						body->hitboxdef.scaleX = object->GetTransform()->GetScale().x;
						body->hitboxdef.scaleY = object->GetTransform()->GetScale().y;
						body->hitboxdef.shape = Box;

						PhysicsWorld* physicsWorld = ObjectHandler::GetInstance().GetPhysicsWorld();
						component = new Physics(body, physicsWorld);
						object->AddComponent(component);
					}
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
				if (hasPhysics)
				{
					object->GetComponent<Physics>()->GetPhysicsBody()->bodyDef.bodyDef.type = (b2BodyType)bodyType;
					object->GetComponent<Physics>()->GetPhysicsBody()->bodyDef.density = density;
					object->GetComponent<Physics>()->GetPhysicsBody()->bodyDef.friction = friction;
				}
			}
			loopNum++;
		}
		ImGui::TreePop();
	}
	ImGui::PushItemWidth(200);

	if(ImGui::InputText("File Name", fileName, 30))
		scene->SetFileName(fileName);


	ImGui::SameLine();
	if (ImGui::Button("Save"))
		scene->Save();

	if (ImGui::Button("Load"))
	{
		std::string sFileName = fileName;
		

		SceneManager::GetInstance().LoadScene(fileName);
	}

	ImGui::End();
#endif
	// ASSEMBLE AND RENDER DRAW DATA
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	this->_swapChain->Present(1, NULL); // FIRST VALUE 1 = VSYNC ON 0 = VYSNC OFF 
}

