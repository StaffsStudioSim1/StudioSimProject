#pragma once

#include "Component.h"
#include "Transform.h"
#include "../Graphics/ConstantBuffer.h"
#include "../nlohmann/json.hpp"
using json = nlohmann::json;

#define JSON_SCENE_BACKGROUND "backgroundImage"
#define JSON_SCENE_GAMEOBJECTS "gameObjects"
#define JSON_SCENE_STAGECOLLISION "stageCollision"

#define JSON_GO_NAME "name"
#define JSON_GO_TAG "tag"
#define JSON_GO_POSITION "position"
#define JSON_GO_ROTATION "rotation"
#define JSON_GO_SCALE "scale"
#define JSON_GO_COMPONENTS "components"

#define JSON_COMPONENT_CLASS "class"
#define JSON_COMPONENT_CONSTRUCTORS "constructors"

#define JSON_TAG_GAMEOBJECT "GameObject"
#define JSON_TAG_STAGECOLLISION "StageCollision"

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

	// Stores a name for the object
	std::string GetName() const noexcept { return _name; }
	void SetName(std::string name) { _name = name; }

	std::string GetTag() const noexcept { return _tag; }

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

	std::vector<Component*> _components;
};

