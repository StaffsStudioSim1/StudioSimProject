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
	if (isFlipped == _flipHorizontal)
		return;
	_scale.x = -_scale.x;

	_flipHorizontal = isFlipped;
}

void Transform::FlipVertical(bool isFlipped)
{
	if (isFlipped == _flipVertical)
		return;
	_scale.y = -_scale.y;

	_flipVertical = isFlipped;
}

void Transform::Update()
{
	// For model
	DirectX::XMStoreFloat4x4(&_world, DirectX::XMMatrixScaling(_scale.x, _scale.y, 1.0f) * DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, _rotation) * DirectX::XMMatrixTranslation(_position.x, _position.y, _depthPos));

	if (_pParent != nullptr)
		DirectX::XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _pParent->GetWorldMatrix());
}
