#pragma once

#include "Appearance.h"
#include "Transform.h"
#include "Component.h"
#include "../nlohmann/json.hpp"
using json = nlohmann::json;

class GameObject
{
public:
	GameObject();
	GameObject(json json);
	~GameObject();

	int GetType() const noexcept { return _type; }
	void SetType(int type) { _type = type; }

	Appearance* GetAppearance() const noexcept { return _pAppearance; }
	void AddAppearance(Appearance* appearance) { _pAppearance = appearance; }

	Transform* GetTransform() const noexcept { return _pTransform; }
	void AddTransform(Transform* transform) { _pTransform = transform; }

	// Stores a name for the object - might not be used
	std::string GetName() const noexcept { return _name; }
	void SetName(std::string name) { _name = name; }

	void Start();
	void Update(float deltaTime);
	void FixedUpdate(float timeStep);
	void Stop();

	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
private:
	int _type = 0;

	std::string _name;

	Appearance* _pAppearance = {};
	Transform* _pTransform = {};

	std::vector<Component*> _components;
};

