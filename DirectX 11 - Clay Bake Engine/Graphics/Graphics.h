#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"

#include "../GameObjects/GameObject.h"
#include "../Includes/DDSTextureLoader.h"

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

	Geometry											squareGeometryData;

	GameObject* pTestObject;
	ID3D11ShaderResourceView* testTexture = nullptr;
};