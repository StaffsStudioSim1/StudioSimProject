#include "GameObject.h"
#include "ObjectHandler.h"
#include "Appearance.h"
#include "Physics.h"
#include "../Input/PlayerInput.h"

static int idNumber = 0; // Gives each object a unique ID number

GameObject::GameObject(std::string name) : GameObject(name, Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f), 0.0f)
{
}

GameObject::GameObject(std::string name, Vector3 position, Vector2 scale, float rotation)
{
	_name = name;
	_transform.SetPosition(position);
	_transform.SetScale(scale);
	_transform.SetRotation(rotation);

	_id = idNumber;
	idNumber++;

	ObjectHandler::GetInstance().Register(this);
}

GameObject::GameObject(json objectJson)
{
	_name = objectJson[JSON_GO_NAME];
	_transform.SetPosition(objectJson[JSON_GO_POSITION].at(0), objectJson[JSON_GO_POSITION].at(1));
	_transform.SetDepthPos(objectJson[JSON_GO_POSITION].at(2));
	_transform.SetScale(objectJson[JSON_GO_SCALE].at(0), objectJson[JSON_GO_SCALE].at(1));
	_transform.SetRotation(DirectX::XMConvertToRadians(objectJson[JSON_GO_ROTATION]));

	_id = idNumber;
	idNumber++;

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
		else if (type == "Physics")
		{
			bool dynamic = componentJson.contains(JSON_COMPONENT_CONSTRUCTORS) && componentJson[JSON_COMPONENT_CONSTRUCTORS].at(0);

			PhysicsBody* body = new PhysicsBody();
			body->bodyDef.startPos = _transform.GetPosition();
			body->bodyDef.startingRoatation = _transform.GetRotation();
			body->bodyDef.density = 0.1f;
			body->bodyDef.friction = 1.0f;
			body->hitboxdef.bodyType = dynamic ? Dynmaic : Static;
			body->hitboxdef.scaleX = _transform.GetScale().x;
			body->hitboxdef.scaleY = _transform.GetScale().y;
			body->hitboxdef.shape = Box;

			PhysicsWorld* physicsWorld = ObjectHandler::GetInstance().GetPhysicsWorld();
			component = new Physics(body, physicsWorld);
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

json GameObject::Write()
{
	json me;
	me[JSON_GO_NAME] = _name;
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
