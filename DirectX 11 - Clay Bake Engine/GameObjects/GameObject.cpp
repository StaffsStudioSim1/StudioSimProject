#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(json json)
{
}

GameObject::~GameObject()
{
	delete _pTransform;
	_pTransform = nullptr;

	delete _pAppearance;
	_pAppearance = nullptr;
}

void GameObject::Start()
{
	for (Component* component : _components)
		component->Start();
}

void GameObject::Update(float deltaTime)
{
	_pTransform->Update();

	if (_pPhysics)
		_pPhysics->Update();
	for (Component* component : _components)
		component->Update(deltaTime);
}

void GameObject::FixedUpdate(float timeStep)
{
	for (Component* component : _components)
		component->FixedUpdate(timeStep);
}

void GameObject::Stop()
{
	for (Component* component : _components)
		component->Stop();
}

void GameObject::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	_pAppearance->Render(context);
}
