#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "ConstantBuffer.h"
#include "../Scene.h"
#include "../GameObjects/GameManager.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <DDSTextureLoader.h>

#define INTERNAL_RESOLUTION_X 648
#define INTERNAL_RESOLUTION_Y 360

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexC;
};

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame(Scene* scene);
private:
	bool InitializeDirectX(HWND hwnd, int width, int hegiht);
	bool InitializeShaders();
	bool InitializeScene();

	void ResizeWindow();

	void LoadSettingsFromFile();

	bool _initialStart = true; // used to prevent memory leaks when resizing the window
	int _windowWidth, _windowHeight;

	// For options - here for the time being
	int _currentResolution = 0;
	int _resolutionWidth, _resolutionHeight;
	bool _useFullscreen = false;
	int _musicVol, _soundVol;
	std::string tempBackgroundTexture;

	SoundEffect* _UISound;

	Microsoft::WRL::ComPtr <ID3D11Device>				_device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext>		_deviceContext;
	Microsoft::WRL::ComPtr <IDXGISwapChain>				_swapChain;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView>		_renderTargertView;

							VertexShader				_vertexshader;
							PixelShader					_pixelshader;

	Microsoft::WRL::ComPtr <ID3D11Buffer>				_vertexBuffer;
	Microsoft::WRL::ComPtr <ID3D11Buffer>				_indexBuffer;

	Microsoft::WRL::ComPtr <ID3D11Buffer>				_constantBuffer;

	Microsoft::WRL::ComPtr <ID3D11Texture2D>			_depthStencilBuffer;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView>		_depthStencilView;

	Microsoft::WRL::ComPtr <ID3D11SamplerState>			_samplerState;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState>	_stencilState;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState>		_wireframeRasterState;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState>		_solidRasterState;
	Microsoft::WRL::ComPtr <ID3D11BlendState>			_blendState;

	DirectX::XMFLOAT4X4									_world;
	DirectX::XMFLOAT4X4									_view;
	DirectX::XMFLOAT4X4									_projection;
};