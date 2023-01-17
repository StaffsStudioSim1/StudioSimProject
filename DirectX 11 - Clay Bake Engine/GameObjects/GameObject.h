#pragma once

#include "Appearance.h"
#include "Physics.h"

class GameObject
{
public:
	GameObject(int type);
	~GameObject();

	enum OBJECT_TYPE
	{
		TYPE_EXAMPLE = 0,
	};

	int GetType() const noexcept { return _type; }
	void SetType(int type) { _type = type; }

	Appearance* GetAppearance() const noexcept { return _pAppearance; }
	void AddAppearance(Appearance* appearance) { _pAppearance = appearance; }

	Transform* GetTransform() const noexcept { return _pTransform; }
	void AddTransform(Transform* transform) { _pTransform = transform; }

	Physics* GetPhysics() const noexcept { return _pPhysics; }
	void AddPhysics(Physics* physics) { _pPhysics = physics; }

	// Stores a name for the object - might not be used
	std::string GetName() const noexcept { return _name; }
	void SetName(std::string name) { _name = name; }

	void Update();
	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
private:
	int _type = 0;

	std::string _name;

	Appearance* _pAppearance = {};
	Transform* _pTransform = {};
	Physics* _pPhysics = {};
};

