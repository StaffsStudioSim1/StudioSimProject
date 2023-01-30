#include "Transform.h"

void Transform::Update()
{
	// For model
	DirectX::XMStoreFloat4x4(&_world, DirectX::XMMatrixScaling(_scale.x, _scale.y, 1.0f) * DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, _rotation) * DirectX::XMMatrixTranslation(_position.x, _position.y, _depthPos));

	if (_pParent != nullptr)
		DirectX::XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _pParent->GetWorldMatrix());
}
