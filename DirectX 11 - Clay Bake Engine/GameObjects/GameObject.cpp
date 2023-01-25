#include "GameObject.h"
#include "ObjectHandler.h"
#include "Appearance.h"
#include "../Input/PlayerInput.h"

GameObject::GameObject(std::string name)
{
	_name = name;
	ObjectHandler::GetInstance().Register(this);
}

GameObject::GameObject(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 scale, float rotation) : GameObject(name)
{
	_transform.SetPosition(position);
	_transform.SetScale(scale);
	_transform.SetRotation(rotation);
}

GameObject::GameObject(json objectJson)
{
	_name = objectJson[JSON_GO_NAME];
	_transform.SetPosition(objectJson[JSON_GO_POSITION].at(0), objectJson[JSON_GO_POSITION].at(1));
	_transform.SetDepthPos(objectJson[JSON_GO_POSITION].at(2));
	_transform.SetScale(objectJson[JSON_GO_SCALE].at(0), objectJson[JSON_GO_SCALE].at(1));
	_transform.SetRotation(DirectX::XMConvertToRadians(objectJson[JSON_GO_ROTATION]));

	for (json componentJson : objectJson[JSON_GO_COMPONENTS])
	{
		Component* component = nullptr;
		std::string type = componentJson[JSON_COMPONENT_CLASS];
		if (type == "Appearance")
		{
			std::string textureName = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(0);
			DirectX::XMFLOAT4 texCoords = { 
				componentJson[JSON_COMPONENT_CONSTRUCTORS].at(1), componentJson[JSON_COMPONENT_CONSTRUCTORS].at(2),
				componentJson[JSON_COMPONENT_CONSTRUCTORS].at(3), componentJson[JSON_COMPONENT_CONSTRUCTORS].at(4)
			};
			float alphaMultiplier = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(5);
			component = new Appearance(textureName, texCoords, alphaMultiplier);
		}
		else if (type == "PlayerController")
		{
			// TODO add yo stuff here
		}

		if (component != nullptr)
			AddComponent(component);
	}
	ObjectHandler::GetInstance().Register(this);
}

GameObject::~GameObject()
{
	for (Component* component : _components)
		delete component;
	_components.clear();

	ObjectHandler::GetInstance().Unregister(this);
}

void GameObject::AddComponent(Component* component)
{
	component->SetObject(this);
	_components.push_back(component);
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
		_physics->Update(deltaTime);

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
	for (Component* component : _components)
		component->Render(context, constantBuffer, globalBuffer);
}
