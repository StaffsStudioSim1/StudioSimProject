#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <wrl/client.h>

// For loaded models
struct Geometry
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	int numOfIndices = 0;

	UINT vertexBufferStride = 0;
	UINT vertexBufferOffset = 0;
};

class Appearance
{
public:
	Appearance();
	~Appearance() {}

	// For loaded models
	Geometry GetGeometryData() const noexcept { return _geometry; }
	void SetGeometryData(Geometry geometry) { _geometry = geometry; }

	// Object texture
	bool HasTexture() const noexcept { return _pTextureRV ? true : false; }
	ID3D11ShaderResourceView* GetTexture() const noexcept { return _pTextureRV.Get(); }
	void SetTexture(ID3D11ShaderResourceView* textureRV) { _pTextureRV = textureRV; }

	DirectX::XMFLOAT4X4 GetTexMatrix() const noexcept { return _texMatrix; }
	DirectX::XMFLOAT4 GetTexCoords() const noexcept { return _texCoords; }
	void SetTexCoords(float numOfXFrames, float numOfYFrames, float xFramePos, float yFramePos); // number of frames contained in texture file and then the position of which frame you want to use
	void SetTexCoords(DirectX::XMFLOAT4 coords) { SetTexCoords(coords.x, coords.y, coords.z, coords.w); }

	float GetAlphaMultiplier() const noexcept { return _alphaMultiplier; }
	void SetAlphaMultiplier(float alpha) { _alphaMultiplier = alpha; }

	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context);
private:
	Geometry _geometry;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pTextureRV;

	DirectX::XMFLOAT4X4 _texMatrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	DirectX::XMFLOAT4 _texCoords = { 1.0f, 1.0f, 0.0f, 0.0f };

	float _alphaMultiplier = 1.0f;
};

