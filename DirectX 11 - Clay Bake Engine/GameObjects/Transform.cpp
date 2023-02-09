#include "Transform.h"

Transform::Transform()
{
	_position = { 0.0f, 0.0f };
	_scale = { 1.0f, 1.0f };
	_depthPos = 0.0f;
	_rotation = 0.0f;
	_pParent = nullptr;
}

void Transform::FlipHorizontal(bool isFlipped)
{
	if (isFlipped != IsFlipTest)
		_scale.x = -_scale.x;
	
	IsFlipTest = isFlipped;
}

void Transform::FlipVertical(bool isFlipped)
{
	if (isFlipped != IsFlipTest)
		_scale.y = -_scale.y;

	IsFlipTest = isFlipped;
}

void Transform::Update()
{
	// For model
	DirectX::XMStoreFloat4x4(&_world, DirectX::XMMatrixScaling(_scale.x, _scale.y, 1.0f) * DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, _rotation) * DirectX::XMMatrixTranslation(_position.x, _position.y, _depthPos));

	if (_pParent != nullptr)
		DirectX::XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _pParent->GetWorldMatrix());
}
