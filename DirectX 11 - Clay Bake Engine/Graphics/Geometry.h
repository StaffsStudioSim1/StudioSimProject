#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

struct Geometry
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	UINT numOfIndices = 0;

	UINT vertexBufferStride = 0;
	UINT vertexBufferOffset = 0;
};