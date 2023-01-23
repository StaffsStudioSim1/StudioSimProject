#include "GameObject.h"
#include "ObjectHandler.h"

GameObject::GameObject(std::string name)
{
	ObjectHandler::GetInstance()->Register(this);
}

GameObject::GameObject(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 scale, float rotation) : GameObject(name)
{
	_transform.SetPosition(position);
	_transform.SetScale(scale);
	_transform.SetRotation(rotation);
}

GameObject::GameObject(json json)
{
	ObjectHandler::GetInstance()->Unregister (this);
}

GameObject::~GameObject()
{
	for (Component* component : _components)
		delete component;
	_components.clear();
}

void GameObject::Start()
{
	for (Component* component : _components)
		component->Start();
}

void GameObject::Update(float deltaTime)
{
	_transform.Update();

	if (_physics)
		_physics->Update();

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

void GameObject::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
	constantBuffer.mWorld = DirectX::XMMatrixTranspose(_transform.GetWorldMatrix());

	for (Component* component : _components)
		component->Render(context, constantBuffer, globalBuffer);
}
