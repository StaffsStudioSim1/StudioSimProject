#pragma once
#include <DirectXMath.h>

struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
	DirectX::XMFLOAT4X4 mTexCoord;
	float mAlphaMultiplier;
};