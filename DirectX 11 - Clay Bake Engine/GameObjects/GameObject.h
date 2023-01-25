#pragma once

#include "Appearance.h"
#include "Physics.h"
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

	template<typename T>
	T* GetComponent()
	{
		for (Component* component : _components)
			if (T* v = dynamic_cast<T*>(component))
				return v;
		return nullptr;
	}
	void AddComponent(Component* component);

	Transform* GetTransform() { return &_transform; }
	Physics* GetPhysics() const noexcept { return _physics; }
	void AddPhysics(Physics* physics) { _physics = physics; }

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
	Physics* _pPhysics = {};

	std::vector<Component*> _components;
};

