#pragma once

#include <DirectXMath.h>

class Transform
{
public:
	Transform();
	~Transform() {}

	DirectX::XMFLOAT2 GetPosition() const noexcept { return _position; }
	void SetPosition(DirectX::XMFLOAT2 position) { _position = position; }
	void SetPosition(float x, float y) { _position.x = x; _position.y = y; }
	void SetPositionChange(DirectX::XMFLOAT2 change) { _position.x += change.x; _position.y += change.y; }
	void SetPositionChange(float x, float y) { _position.x += x; _position.y += y; }

	float GetRotation() const noexcept { return _rotation; }
	void SetRotation(float rotation) { _rotation = rotation; }
	void SetRotationChange(float change) { _rotation += change; }

	DirectX::XMFLOAT2 GetScale() const noexcept { return _scale; }
	void SetScale(DirectX::XMFLOAT2 scale) { _scale = scale; }
	void SetScale(float x, float y) { _scale.x = x; _scale.y = y; }
	void SetScaleChange(DirectX::XMFLOAT2 change) { _scale.x += change.x; _scale.y += change.y; }
	void SetScaleChange(float x, float y) { _scale.x += x; _scale.y += y; }
	// Add way to keep aspect ratio/link x and y scale

	DirectX::XMMATRIX GetWorldMatrix() const noexcept { return DirectX::XMLoadFloat4x4(&_world); }

	bool GetHasTransformParent() const noexcept { return _pParent ? true : false; }
	void SetTransformParent(Transform* parent) { _pParent = parent; }

	void Update();
private:
	DirectX::XMFLOAT2 _position, _scale;
	float _rotation;

	Transform* _pParent;

	DirectX::XMFLOAT4X4 _world{};
};

