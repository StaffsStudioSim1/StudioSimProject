#pragma once

#include "Components/Component.h"
#include "Transform.h"
#include "../Graphics/ConstantBuffer.h"
#include "../nlohmann/json.hpp"
using json = nlohmann::json;

class GameObject
{
public:
	GameObject(std::string name);
	GameObject(std::string name, Vector3 position, Vector2 scale, float rotation);
	GameObject(json objectJson);
	~GameObject();

	json Write();

	enum OBJECT_TYPE
	{
		OBJECT_DEFAULT = 0,
		OBJECT_STATIC,
		OBJECT_DYNAMIC,
	};

	int GetType() const noexcept { return _type; }
	void SetType(int type) { _type = type; }

	int GetID() const noexcept { return _id; }
	void SetID(int id) { _id = id; }

	template<typename T>
	T* GetComponent()
	{
		for (Component* component : _components)
			if (T* v = dynamic_cast<T*>(component))
				return v;
		return nullptr;
	}
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	Transform* GetTransform() { return &_transform; }
	bool HasRigidbody() { return _hasRigidbody; };
	bool HasCollider() { return _hasCollider; };

	// Stores a name for the object
	std::string GetName() const noexcept { return _name; }
	void SetName(std::string name) { _name = name; }

	std::string GetTag() const noexcept { return _tag; }
	void SetTag(std::string tag) { _tag = tag; }

	void Start();
	void Update(float deltaTime);
	void FixedUpdate(float timeStep);
	void Stop();

	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer);
private:
	int _type = 0;
	int _id = 0;

	std::string _name;
	std::string _tag;

	Transform _transform = {};
	bool _hasRigidbody;
	bool _hasCollider;

	std::vector<Component*> _components;
};

