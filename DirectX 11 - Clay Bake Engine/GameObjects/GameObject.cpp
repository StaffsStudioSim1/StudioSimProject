#include "GameObject.h"
#include "ObjectHandler.h"
#include "Components/Appearance.h"
#include "Components/Interactable.h"
#include "Components/LeverComponent.h"
#include "Components/DoorComponent.h"
#include "Components/ButtonComponent.h"
#include "Components/Appearance.h"
#include "Components/PressurePlateComponent.h"
#include "Components/Goal.h"
#include "PlayerController.h"
#include "../Input/PlayerInput.h"
#include "GameManager.h"
#include "PlayerMagnetism.h"
#include "MovingPlatform.h"
#include "MagnetBox.h"

GameObject::GameObject(std::string name) : GameObject(name, Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f), 0.0f)
{
}

GameObject::GameObject(std::string name, Vector3 position, Vector2 scale, float rotation)
{
	_name = name;
	_transform.SetPosition(position);
	_transform.SetScale(scale);
	_transform.SetRotation(rotation);

	_id = ObjectHandler::GetInstance().SetObjectID();

	ObjectHandler::GetInstance().Register(this);
}

GameObject::GameObject(json objectJson)
{
	_name = objectJson[JSON_GO_NAME];
	_transform.SetPosition(objectJson[JSON_GO_POSITION].at(0), objectJson[JSON_GO_POSITION].at(1));
	_transform.SetDepthPos(objectJson[JSON_GO_POSITION].at(2));
	_transform.SetScale(objectJson[JSON_GO_SCALE].at(0), objectJson[JSON_GO_SCALE].at(1));
	_transform.SetRotation(DirectX::XMConvertToRadians(objectJson[JSON_GO_ROTATION]));

	_id = ObjectHandler::GetInstance().SetObjectID();

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
			Vector2 offset;
			if (componentJson[JSON_COMPONENT_CONSTRUCTORS].size() > 6)
			{
				offset.x = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(6);
				offset.y = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(7);
			}
			component = new Appearance(textureName, texCoords, alphaMultiplier, offset);
		}
		else if (type == "PlayerController")
		{
			int playerID = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(0);
			component = new PlayerController(playerID);
		}
		else if (type == "LeverComponent")
		{
			int switchType = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(0);
			std::string linkedObject = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(1);

			component = new LeverComponent((Interactable::InteractableLink)switchType, linkedObject);
		}
		else if (type == "PressurePlateComponent")
		{
			int switchType = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(0);
			std::string linkedObject = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(1);

			component = new PressurePlateComponent((Interactable::InteractableLink)switchType, linkedObject);
		}
		else if (type == "DoorComponent")
		{

			bool doorOpenDefault = false;
			if (componentJson.contains(JSON_COMPONENT_CONSTRUCTORS))
			{
				doorOpenDefault = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(0);
			}

			component = new DoorComponent(doorOpenDefault);
		}
		else if (type == "Goal")
		{
			std::string levelName = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(0);

			component = new Goal(levelName);
		}
		else if (type == "ButtonComponent")
		{
			int switchType = 0;
			std::string linkedObject = "";
			component = new ButtonComponent((Interactable::InteractableLink)switchType, linkedObject);
		}
		else if (type == "PressurePlateComponent")
		{
			int switchType = 0;
			std::string linkedObject = "";
			component = new PressurePlateComponent((Interactable::InteractableLink)switchType, linkedObject);
		}
		else if (type == "Rigidbody")
		{
			component = new Rigidbody();
		}
		else if (type == "AABB")
		{
			float width = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(0);
			float height = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(1);
			bool trigger = false;
			if (componentJson[JSON_COMPONENT_CONSTRUCTORS].size() > 2)
				trigger = componentJson[JSON_COMPONENT_CONSTRUCTORS].at(2);
			component = new AABB(width, height, trigger);
		}

		else if (type == "MagnetBox")
		{
			component = new MagnetBox();
		}
		else if (type == "MovingPlatform")
		{

			if (componentJson.contains(JSON_COMPONENT_CONSTRUCTORS))
			{
				component = new MovingPlatform(componentJson[JSON_COMPONENT_CONSTRUCTORS].at(0), componentJson[JSON_COMPONENT_CONSTRUCTORS].at(1), componentJson[JSON_COMPONENT_CONSTRUCTORS].at(2), componentJson[JSON_COMPONENT_CONSTRUCTORS].at(3));
			}
			else
			{
				component = new MovingPlatform(100.0f, 0, 100.0f, 0);

			}

		}
		else if (type == "PlayerMagnetism")
		{
			component = new PlayerMagnetism();
		}

		if (component != nullptr)
			AddComponent(component);
	}

	if (objectJson.contains(JSON_GO_TAG))
		_tag = objectJson[JSON_GO_TAG];
	else
		_tag = JSON_TAG_GAMEOBJECT;

	ObjectHandler::GetInstance().Register(this);
}

GameObject::~GameObject()
{
	for (Component* component : _components)
		delete component;
	_components.clear();

	ObjectHandler::GetInstance().Unregister(this);
}

json GameObject::Write()
{
	json me;
	me[JSON_GO_NAME] = _name;
	me[JSON_GO_TAG] = _tag;
	me[JSON_GO_POSITION] = { _transform.GetPosition().x, _transform.GetPosition().y, _transform.GetDepthPos() };
	me[JSON_GO_ROTATION] = DirectX::XMConvertToDegrees(_transform.GetRotation());
	me[JSON_GO_SCALE] = { _transform.GetScale().x, _transform.GetScale().y };

	for (Component* component : _components)
	{
		me[JSON_GO_COMPONENTS].push_back(component->Write());
	}

	return me;
}

void GameObject::AddComponent(Component* component)
{
	if (dynamic_cast<AABB*>(component))
		_hasCollider = true;
	else if (dynamic_cast<Rigidbody*>(component))
		_hasRigidbody = true;

	component->SetGameObject(this);
	_components.push_back(component);
}

void GameObject::RemoveComponent(Component* component)
{
	int loopNum = 0;
	for (Component* com : _components)
	{
		if (com == component)
		{
			component->Stop(); // Used to free memory if needed
			_components.erase(_components.begin() + loopNum);
			return;
		}
		loopNum++;
	}
}

void GameObject::Start()
{
	for (Component* component : _components)
		component->Start();
}

void GameObject::Update(float deltaTime)
{
	_transform.Update();

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
