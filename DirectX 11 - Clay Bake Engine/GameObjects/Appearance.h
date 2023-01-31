#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>
#include "Component.h"
#include "../Graphics/Geometry.h"
#include "../Graphics/TextureInfo.h"

class Appearance : public Component
{
public:
	Appearance(std::string textureName = "", DirectX::XMFLOAT4 texCoords = { 1.0f, 1.0f, 0.0f, 0.0f }, float alphaMultiplier = 1.0f);
	~Appearance();

	json Write();

	// For loaded models
	Geometry GetGeometryData() const noexcept { return _geometry; }
	void SetGeometryData(Geometry geometry) { _geometry = geometry; }

	// Object texture
	bool HasTexture() const noexcept { return _texture.texture ? true : false; }
	TextureInfo GetTexture() const noexcept { return _texture; }
	void SetTexture(TextureInfo textureRV) { _texture = textureRV; }

	DirectX::XMFLOAT4X4 GetTexMatrix() const noexcept { return _texMatrix; }
	DirectX::XMFLOAT4 GetTexCoords() const noexcept { return _texCoords; }
	void SetTexCoords(float numOfXFrames, float numOfYFrames, float xFramePos, float yFramePos); // number of frames contained in texture file and then the position of which frame you want to use
	void SetTexCoords(DirectX::XMFLOAT4 coords) { SetTexCoords(coords.x, coords.y, coords.z, coords.w); }
	void SetTexPosChange(float xPos, float yPos) { SetTexCoords(_texCoords.x, _texCoords.y, _texCoords.z += xPos, _texCoords.w += yPos); }

	float GetAlphaMultiplier() const noexcept { return _alphaMultiplier; }
	void SetAlphaMultiplier(float alpha) { _alphaMultiplier = alpha; }

	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);
private:
	Geometry _geometry;
	TextureInfo _texture;

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

