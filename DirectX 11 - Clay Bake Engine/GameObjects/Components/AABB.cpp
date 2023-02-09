#include "AABB.h"
#include "Rigidbody.h"
#include "../GameObject.h"

AABB::AABB(float width, float height)
{
	_width = width;
	_height = height;

#if DEBUG
	_debugGeo = ObjectHandler::GetInstance().GetSquareGeometry();
	_debugTex = ObjectHandler::GetInstance().LoadDDSTextureFile("Resources/Sprites/StageCollision.dds");
#endif
}

json AABB::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "AABB";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_width);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(_height);
	return me;
}

bool AABB::Overlaps(AABB* collider, float deltaTime)
{
	if (!_active || !collider->_active)
		return false;

	DirectX::BoundingBox me;
	me.Center.x = _gameObject->GetTransform()->GetPosition().x;
	me.Center.y = _gameObject->GetTransform()->GetPosition().y;
	me.Center.z = 0.0f;
	me.Extents.x = _width / 2;
	me.Extents.y = _height / 2;
	me.Extents.z = 0.0f;

	DirectX::BoundingBox other;
	other.Center.x = collider->GetGameObject()->GetTransform()->GetPosition().x;
	other.Center.y = collider->GetGameObject()->GetTransform()->GetPosition().y;
	other.Center.z = 0.0f;
	other.Extents.x = collider->_width / 2;
	other.Extents.y = collider->_height / 2;
	other.Extents.z = 0.0f;

	return me.Intersects(other);
}

Vector2 AABB::GetSize()
{
	return Vector2(_width, _height);
}

void AABB::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
#if DEBUG
	if (!_active)
		return;

	DirectX::XMMATRIX world =
		DirectX::XMMatrixScaling(_gameObject->GetTransform()->GetScale().x * _width / 2, _gameObject->GetTransform()->GetScale().y * _height / 2, 1.0f)
		* DirectX::XMMatrixTranslation(_gameObject->GetTransform()->GetPosition().x, _gameObject->GetTransform()->GetPosition().y, _gameObject->GetTransform()->GetDepthPos());

	if (_gameObject->GetTransform()->HasTransformParent())
		world = world * _gameObject->GetTransform()->GetTransformParent()->GetWorldMatrix();

	constantBuffer.mWorld = DirectX::XMMatrixTranspose(world);
	constantBuffer.mTexCoord = {
		1.0f, 0.0f, 0.0f, _width,
		0.0f, 1.0f, 0.0f, _height,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	constantBuffer.mAlphaMultiplier = 1.0f;

	context->UpdateSubresource(globalBuffer.Get(), 0, nullptr, &constantBuffer, 0, 0);

	// Draw object
	context->PSSetShaderResources(0, 1, &_debugTex.texture);
	context->IASetVertexBuffers(0, 1, _debugGeo.vertexBuffer.GetAddressOf(), &_debugGeo.vertexBufferStride, &_debugGeo.vertexBufferOffset);
	context->IASetIndexBuffer(_debugGeo.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->DrawIndexed(_debugGeo.numOfIndices, 0, 0);
#endif
}
