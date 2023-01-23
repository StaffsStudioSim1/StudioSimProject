#include "Graphics.h"
#include "../GameObjects/ObjectHandler.h"

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
	DirectX::XMStoreFloat4x4(&_projection, DirectX::XMMatrixOrthographicLH(width, height, 0.01f, 100.0f));

	if (FAILED(DirectX::CreateDDSTextureFromFile(this->device.Get(), L"Textures\\Test.dds", nullptr, &this->testTexture)))
		exit(-1);

	ObjectHandler::GetInstance()->AddTextureToMap("Test", this->testTexture);

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

		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE; 
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


		hr =D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, // IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,							// Graphics device
			NULL,												// software driver type 
			D3D11_CREATE_DEVICE_DEBUG,							// feature lvls array
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

		this->device->CreateTexture2D(&depthStencilDesc, nullptr, _depthStencilBuffer.GetAddressOf());
		this->device->CreateDepthStencilView(_depthStencilBuffer.Get(), nullptr, _depthStencilView.GetAddressOf());
		this->deviceContext->OMSetRenderTargets(1, this->renderTargertView.GetAddressOf(), _depthStencilView.Get());
			
		//create viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		if (width > height)
			viewport.TopLeftY = -height / 2.5f;
		else
			viewport.TopLeftY = 0;
		viewport.Width = width;
		viewport.Height = width;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		//set viewport
		this->deviceContext->RSSetViewports(1, &viewport); // can add additional view-ports via this 
		
		// Create stencil state
		D3D11_DEPTH_STENCIL_DESC stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		stencilDesc.StencilEnable = false;

		this->device->CreateDepthStencilState(&stencilDesc, _stencilState.GetAddressOf());
		this->deviceContext->OMSetDepthStencilState(_stencilState.Get(), 1);

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
		this->device->CreateSamplerState(&sampDesc, _samplerState.GetAddressOf());

		this->deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		// Create rasteriser states - wireframe and solid
		D3D11_RASTERIZER_DESC rasterDesc0 = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		ZeroMemory(&rasterDesc0, sizeof(D3D11_RASTERIZER_DESC));
		rasterDesc0.MultisampleEnable = true;
		rasterDesc0.AntialiasedLineEnable = true;
		rasterDesc0.FillMode = D3D11_FILL_WIREFRAME;
		rasterDesc0.CullMode = D3D11_CULL_NONE;
		this->device->CreateRasterizerState(&rasterDesc0, _wireframeRasterState.GetAddressOf());

		D3D11_RASTERIZER_DESC rasterDesc1 = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		ZeroMemory(&rasterDesc1, sizeof(D3D11_RASTERIZER_DESC));
		rasterDesc1.MultisampleEnable = true;
		rasterDesc1.AntialiasedLineEnable = true;
		rasterDesc1.FillMode = D3D11_FILL_SOLID;
		rasterDesc1.CullMode = D3D11_CULL_BACK;
		this->device->CreateRasterizerState(&rasterDesc1, _solidRasterState.GetAddressOf());

		this->deviceContext->RSSetState(_solidRasterState.Get());

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
		this->device->CreateBlendState(&bDesc, _blendState.GetAddressOf());
		this->deviceContext->OMSetBlendState(_blendState.Get(), NULL, 0xFFFFFFFF);

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
			{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	
	return true;
}

bool Graphics::InitializeScene()
{
	// Create a primitive square
	SimpleVertex v[] =
	{
		{ DirectX::XMFLOAT3(-0.1f, 0.1f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.1f, 0.1f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.1f, -0.1f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.1f, -0.1f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
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

	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf());
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
	hr = this->device->CreateBuffer(&indexBufferDesc, &indexBufferData, this->indexBuffer.GetAddressOf());

	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	this->device->CreateBuffer(&bd, nullptr, _constantBuffer.GetAddressOf());

	// Save the square shape data
	ObjectHandler::GetInstance()->SetSquareGeometry(this->vertexBuffer, this->indexBuffer, ARRAYSIZE(indices), 0, sizeof(SimpleVertex));

	return true;
}

void Graphics::RenderFrame(Scene* scene)
{
	float bgcolor[] = {1.0f, 0.0f, 1.0f, 1.0f};
	//this->deviceContext->OMSetRenderTargets(1, this->renderTargertView.GetAddressOf(), NULL);
	this->deviceContext->ClearRenderTargetView(this->renderTargertView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	DirectX::XMMATRIX worldMatrix = XMLoadFloat4x4(&_world);
	DirectX::XMMATRIX viewMatrix = XMLoadFloat4x4(&_view);
	DirectX::XMMATRIX projectionMatrix = XMLoadFloat4x4(&_projection);

	ConstantBuffer cb;
	cb.mView = DirectX::XMMatrixTranspose(viewMatrix);
	cb.mProjection = DirectX::XMMatrixTranspose(projectionMatrix);

	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

	this->deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	this->deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

	/*for (std::pair<std::string, GameObject*> object : ObjectHandler::Instance()->GetAllObjects())
	{
		object.second->Update(0.0f);
		cb.mWorld = DirectX::XMMatrixTranspose(object.second->GetTransform()->GetWorldMatrix());
		cb.mTexCoord = object.second->GetAppearance()->GetTexMatrix();
		cb.mAlphaMultiplier = object.second->GetAppearance()->GetAlphaMultiplier();

		this->deviceContext->UpdateSubresource(_constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

		object.second->Render(this->deviceContext);
	}*/
	scene->Render(this->deviceContext, cb, _constantBuffer);

	this->swapChain->Present(1, NULL); // FIRST VALUE 1 = VSYNC ON 0 = VYSNC OFF 
}

