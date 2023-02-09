#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include "../../Graphics/ConstantBuffer.h"
#include "../../nlohmann/json.hpp"
#include "../../Defines.h"
using nlohmann::json;

class GameObject;

class Component
{
public:
	virtual json Write();

	void SetGameObject(GameObject* gameObject);
	GameObject* GetGameObject();

	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float timeStep);
	virtual void Stop();

	virtual void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);

	bool IsActive();
	void SetActive(bool active);
protected:
	GameObject* _gameObject = nullptr;
	bool _active = true;
};

