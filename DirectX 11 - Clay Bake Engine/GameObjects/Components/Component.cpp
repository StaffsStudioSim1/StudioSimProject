#include "Component.h"
#include "../GameObject.h"

json Component::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "NOT_OVERRIDDEN";
	return me;
}

void Component::SetGameObject(GameObject* gameObject)
{
	_gameObject = gameObject;
}

GameObject* Component::GetGameObject()
{
	return _gameObject;
}

void Component::Start()
{
}

void Component::Update(float deltaTime)
{
}

void Component::FixedUpdate(float timeStep)
{
}

void Component::Stop()
{
}

void Component::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
}

bool Component::IsActive()
{
	return _active;
}

void Component::SetActive(bool active)
{
	_active = active;
}
