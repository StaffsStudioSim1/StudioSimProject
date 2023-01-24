#include "Component.h"
#include "../GameObject.h"

void Component::SetObject(GameObject* gameObject)
{
	_gameObject = gameObject;
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
