#pragma once

#include "Components/Component.h"
#include "Transform.h"
#include "Components/Physics.h"
#include "../Graphics/ConstantBuffer.h"
#include "../nlohmann/json.hpp"
using json = nlohmann::json;

#define JSON_GO_NAME "name"
#define JSON_GO_POSITION "position"
#define JSON_GO_ROTATION "rotation"
#define JSON_GO_SCALE "scale"
#define JSON_GO_COMPONENTS "components"

#define JSON_COMPONENT_CLASS "class"
#define JSON_COMPONENT_CONSTRUCTORS "constructors"

class GameObject
{
public:
	GameObject(std::string name);
	GameObject(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 scale, float rotation);
	GameObject(json objectJson);
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

	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);
private:
	int _type = 0;

	std::string _name;

	Transform _transform = {};
	Physics* _physics = {};

	std::vector<Component*> _components;
};

