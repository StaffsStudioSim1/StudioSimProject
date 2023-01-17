#include "GameObject.h"

GameObject::GameObject(int type)
{
	_name = "";
}

GameObject::~GameObject()
{
	delete _pTransform;
	_pTransform = nullptr;

	delete _pAppearance;
	_pAppearance = nullptr;
}

void GameObject::Update()
{
	_pTransform->Update();
}

void GameObject::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	_pAppearance->Render(context);
}
