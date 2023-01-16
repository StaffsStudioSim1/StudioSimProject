#pragma once
#include "AdapterReader.h"
#include "Shaders.h"

#include "../GameObjects/GameObject.h"
#include "../Includes/DDSTextureLoader.h"

struct ConstantBufferStruct
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
	DirectX::XMFLOAT4X4 mTexCoord;
};

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexC;
};

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
private:
	bool InitializeDirectX(HWND hwnd, int width, int hegiht);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr <ID3D11Device>				device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext>		deviceContext;
	Microsoft::WRL::ComPtr <IDXGISwapChain>				swapChain;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView>		renderTargertView;

							VertexShader				vertexshader;
							PixelShader					pixelshader;

	Microsoft::WRL::ComPtr <ID3D11Buffer>				vertexBuffer;
	Microsoft::WRL::ComPtr <ID3D11Buffer>				indexBuffer;

	Microsoft::WRL::ComPtr <ID3D11Buffer>				_constantBuffer;

	Microsoft::WRL::ComPtr <ID3D11Texture2D>			_depthStencilBuffer;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView>		_depthStencilView;

	Microsoft::WRL::ComPtr <ID3D11SamplerState>			_samplerState;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState>		_wireframeRasterState;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState>		_solidRasterState;

	DirectX::XMFLOAT4X4									_world;
	DirectX::XMFLOAT4X4									_view;
	DirectX::XMFLOAT4X4									_projection;

	Geometry											squareGeometryData;

	GameObject* pTestObject;
	ID3D11ShaderResourceView* testTexture = nullptr;
};