#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "../../Graphics/ConstantBuffer.h"

class GameObject;

class Component
{
public:
	void SetObject(GameObject* gameObject);

	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float timeStep);
	virtual void Stop();

	virtual void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);
protected:
	GameObject* _gameObject;
};
