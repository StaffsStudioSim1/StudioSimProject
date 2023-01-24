#include "Appearance.h"
#include "ObjectHandler.h"

Appearance::Appearance(std::string textureName, DirectX::XMFLOAT4 texCoords, float alphaMultiplier)
{
	SetTexture(ObjectHandler::GetInstance().LoadDDSTextureFile(textureName));
	SetTexCoords(texCoords);
	SetAlphaMultiplier(alphaMultiplier);
	SetGeometryData(ObjectHandler::GetInstance().GetSquareGeometry());
}

Appearance::~Appearance()
{
}

void Appearance::SetTexCoords(float numOfXFrames, float numOfYFrames, float xFramePos, float yFramePos)
{
	//Width and height gives an area of the texture
	//x and y are the starting coordinates of the area
	//DirectX UV coordinates are normally 0,0 in the top left and 1,1 in the bottom right

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

void Appearance::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
	DirectX::XMMATRIX world =
		DirectX::XMMatrixScaling(_gameObject->GetTransform()->GetScale().x * _texture.width * _texCoords.x, _gameObject->GetTransform()->GetScale().y * _texture.height * _texCoords.y, 1.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, _gameObject->GetTransform()->GetRotation()) *
		DirectX::XMMatrixTranslation(_gameObject->GetTransform()->GetPosition().x, _gameObject->GetTransform()->GetPosition().y, _gameObject->GetTransform()->GetDepthPos());

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
