#pragma once

#include <DirectXMath.h>
#include "..\Physics\Vectors.h"

class Transform
{
public:
	Transform();
	~Transform() {}

	DirectX::XMFLOAT2 GetPosition() const noexcept { return _position; }
	Vector2 GetPosition() { Vector2 pos; float a, b; a = _position.x; b = _position.y; pos.x = a; pos.y = b; return pos; }

	void SetPosition(DirectX::XMFLOAT3 position) { _position.x = position.x; _position.y = position.y; _depthPos = position.z; }
	void SetPosition(Vector3 position) { _position.x = position.x; _position.y = position.y; _depthPos = position.z; }
	void SetPosition(DirectX::XMFLOAT2 position) { _position.x = position.x; _position.y = position.y; }
	void SetPosition(Vector2 position) { _position.x = position.x; _position.y = position.y; }
	void SetPosition(float x, float y) { _position.x = x; _position.y = y; }
	void SetPositionChange(DirectX::XMFLOAT2 change) { _position.x += change.x; _position.y += change.y; }
	void SetPositionChange(Vector2 change) { _position.x += change.x; _position.y += change.y; }
	void SetPositionChange(float x, float y) { _position.x += x; _position.y += y; }

	float GetDepthPos() const noexcept { return _depthPos; }
	void SetDepthPos(float depth) { _depthPos = depth; }

	float GetRotation() const noexcept { return _rotation; }
	void SetRotation(float rotation) { _rotation = rotation; }
	void SetRotationChange(float change) { _rotation += change; }

	DirectX::XMFLOAT2 GetScale() const noexcept { return _scale; }
	Vector2 GetVectorScale() { return Vector2(_scale.x, _scale.y); }
	void SetScale(DirectX::XMFLOAT2 scale) { _scale = scale; }
	void SetScale(Vector2 scale) { _scale.x = scale.x; _scale.y = scale.y; }
	void SetScale(float x, float y) { _scale.x = x; _scale.y = y; }
	void SetScaleChange(DirectX::XMFLOAT2 change) { _scale.x += change.x; _scale.y += change.y; }
	void SetScaleChange(Vector2 change) { _scale.x += change.x; _scale.y += change.y; }
	void SetScaleChange(float x, float y) { _scale.x += x; _scale.y += y; }
	// Add way to keep aspect ratio/link x and y scale

	DirectX::XMMATRIX GetWorldMatrix() const noexcept { return DirectX::XMLoadFloat4x4(&_world); }

	bool HasTransformParent() const noexcept { return _pParent ? true : false; }
	Transform* GetTransformParent() const noexcept { return _pParent; }
	void SetTransformParent(Transform* parent) { _pParent = parent; }

	void Update();
private:
	DirectX::XMFLOAT2 _position, _scale;
	float _rotation, _depthPos;

	Transform* _pParent;

	DirectX::XMFLOAT4X4 _world{};
};

