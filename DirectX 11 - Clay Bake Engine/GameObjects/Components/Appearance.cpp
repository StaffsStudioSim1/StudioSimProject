#include "Appearance.h"
#include "../ObjectHandler.h"

Appearance::Appearance(std::string textureName, DirectX::XMFLOAT4 texCoords, float alphaMultiplier, Vector2 offset)
{
	SetTexture(ObjectHandler::GetInstance().LoadDDSTextureFile(textureName));
	SetTexCoords(texCoords);
	SetAlphaMultiplier(alphaMultiplier);
	SetGeometryData(ObjectHandler::GetInstance().GetSquareGeometry());
	_offset = offset;
}

Appearance::~Appearance()
{
}

json Appearance::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "Appearance";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_texture.filePath);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_numOfXFrames);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_numOfYFrames);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_xFramePos);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_yFramePos);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_alphaMultiplier);
	if (_offset.x != 0 || _offset.y != 0)
	{
		me[JSON_COMPONENT_CONSTRUCTORS].push_back(_offset.x);
		me[JSON_COMPONENT_CONSTRUCTORS].push_back(_offset.y);
	}
	return me;
}

void Appearance::SetTexCoords(float numOfXFrames, float numOfYFrames, float xFramePos, float yFramePos)
{
	//Width and height gives an area of the texture
	//x and y are the starting coordinates of the area
	//DirectX UV coordinates are normally 0,0 in the top left and 1,1 in the bottom right
	_numOfXFrames = numOfXFrames;
	_numOfYFrames = numOfYFrames;
	_xFramePos = xFramePos;
	_yFramePos = yFramePos;

	float width = 1.0f / numOfXFrames;
	float height = 1.0f / numOfYFrames;
	float x = width * xFramePos;
	float y = height * yFramePos;

	_texMatrix =
	{
		width, 0.0f, 0.0f, x,
		0.0f, height, 0.0f, y,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	_texCoords = { width, height, x, y };
}

void Appearance::UpdateMatrix()
{
	_texMatrix =
	{
		_texCoords.x, 0.0f, 0.0f, _texCoords.z,
		0.0f ,_texCoords.y, 0.0f, _texCoords.w,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
}

void Appearance::NextXFrame(bool moveLeft)
{
	DirectX::XMFLOAT4 texInfo = GetTexCoordFrameValues();

	if (moveLeft)
		texInfo.z += 1;
	else
		texInfo.z -= 1;

	SetTexCoords(texInfo);
}

void Appearance::NextYFrame(bool moveDown)
{
	DirectX::XMFLOAT4 texInfo = GetTexCoordFrameValues();

	if (moveDown)
		texInfo.w += 1;
	else
		texInfo.w -= 1;

	SetTexCoords(texInfo);
}

void Appearance::FlipTextureOnYAxis()
{
	_texCoords.z += 1;
	UpdateMatrix();
}

void Appearance::FlipTextureOnXAxis()
{
	_texCoords.w += 1;
	UpdateMatrix();
}

void Appearance::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
	if (!_active)
		return;

	DirectX::XMMATRIX world =
		DirectX::XMMatrixScaling(_gameObject->GetTransform()->GetScale().x * _texture.width * _texCoords.x / 2, _gameObject->GetTransform()->GetScale().y * _texture.height * _texCoords.y / 2, 1.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, _gameObject->GetTransform()->GetRotation())
		* DirectX::XMMatrixTranslation(_gameObject->GetTransform()->GetPosition().x + _offset.x, _gameObject->GetTransform()->GetPosition().y + _offset.y, _gameObject->GetTransform()->GetDepthPos());

	if (_gameObject->GetTransform()->HasTransformParent())
		world = world * _gameObject->GetTransform()->GetTransformParent()->GetWorldMatrix();

	constantBuffer.mWorld = DirectX::XMMatrixTranspose(world);
	constantBuffer.mTexCoord = GetTexMatrix();
	constantBuffer.mAlphaMultiplier = GetAlphaMultiplier();

	context->UpdateSubresource(globalBuffer.Get(), 0, nullptr, &constantBuffer, 0, 0);

	// Draw object
	context->PSSetShaderResources(0, 1, &_texture.texture);
	context->IASetVertexBuffers(0, 1, _geometry.vertexBuffer.GetAddressOf(), &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	context->IASetIndexBuffer(_geometry.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->DrawIndexed(_geometry.numOfIndices, 0, 0);
}
