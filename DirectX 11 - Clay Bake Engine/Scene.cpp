#include "Scene.h"
#include <fstream>
#include "ErrorLogger.h"
#include "nlohmann/json.hpp"
#include "GameObjects/ObjectHandler.h"
using json = nlohmann::json;

#if EDIT_MODE
#include "Input/InputManager.h"
#include "GameObjects/ObjectHandler.h"
#endif

Scene::Scene(std::string filePath)
{
	std::ifstream f(filePath);
	if (!f.good())
		ErrorLogger::Log("Unable to find scene file " + filePath);

	json data = json::parse(f);

	std::string image = data[JSON_SCENE_BACKGROUND];

	for (json objectData : data[JSON_SCENE_GAMEOBJECTS])
		_children.push_back(new GameObject(objectData));

#if EDIT_MODE
	RECT rc;
	GetWindowRect(GetActiveWindow(), &rc);
	int width = (rc.right - rc.left) - 16;
	int height = (rc.bottom - rc.top) - 39;
	_mousePicking.Initialise(width, height);
#endif
}

Scene::~Scene()
{
	for (GameObject* obj : _children)
		delete obj;
	_children.clear();
}

void Scene::Save()
{
	json scene;
	json gameObjects;

	scene[JSON_SCENE_BACKGROUND] = "";

	for (GameObject* obj : _children)
	{
		gameObjects.push_back(obj->Write());
	}

	scene["gameObjects"] = gameObjects;

	std::ofstream o("Resources/saved_scene.json");
	o << std::setw(4) << scene << std::endl;
	o.close();
}

void Scene::Start()
{
	for (GameObject* obj : _children)
		obj->Start();
}

void Scene::Update(float deltaTime)
{
#if EDIT_MODE
	static int selectedObj = -1;
	static DirectX::XMINT2 startingPos = { 0, 0 };

	MouseClass* mouse = InputManager::GetInstance().GetMouse();
	DirectX::XMINT2 mousePos = { mouse->GetPosX(), mouse->GetPosY() };

	while (!mouse->EventBufferIsEmpty()) // Handles moving, creating and deleting objects with the mouse in edit mode
	{
		MouseEvent me = mouse->ReadEvent();
		if (me.GetType() == MouseEvent::EventType::LPress && selectedObj == -1) // Probably can be changed to a switch statement
		{
			selectedObj = _mousePicking.TestForObjectIntersection(mousePos.x, mousePos.y, selectedObj);
			if (selectedObj != -1)
			{
				Vector2 pos = ObjectHandler::GetInstance().GetGameObject(selectedObj)->GetTransform()->GetPosition();
				startingPos = { (int)pos.x, (int)pos.y };
			}
		}
		else if (me.GetType() == MouseEvent::EventType::LRelease && selectedObj != -1)
		{
			GameObject* object = ObjectHandler::GetInstance().GetGameObject(selectedObj);
			Vector2 objectPos = object->GetTransform()->GetPosition();
			DirectX::XMINT2 snapPos = _mousePicking.SnapCoordinatesToGrid(objectPos.x, objectPos.y);

			for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
			{
				if (object != ObjectHandler::GetInstance().GetGameObject(selectedObj) && object->GetTransform()->GetPosition().x == snapPos.x && object->GetTransform()->GetPosition().y == snapPos.y)
				{
					ObjectHandler::GetInstance().GetGameObject(selectedObj)->GetTransform()->SetPosition(startingPos.x, startingPos.y);
					selectedObj = -1;
					return;
				}
			}

			object->GetTransform()->SetPosition(snapPos.x, snapPos.y);
			selectedObj = -1;
		}
		else if (selectedObj != -1)
		{
			GameObject* object = ObjectHandler::GetInstance().GetGameObject(selectedObj);
			DirectX::XMINT2 relativeMousePos = _mousePicking.GetRelativeMousePos(mousePos.x, mousePos.y);
			object->GetTransform()->SetPosition(relativeMousePos.x, relativeMousePos.y);

			if (me.GetType() == MouseEvent::EventType::MPress) // Remove the currently selected game object
			{
				_children.erase(_children.begin() + selectedObj);
				ObjectHandler::GetInstance().Unregister(object);
				selectedObj = -1;
			}
		}
		else if (me.GetType() == MouseEvent::EventType::RPress && selectedObj == -1) // Creates a new game object
		{
			static int objNum = 0;
			DirectX::XMINT2 relPos = _mousePicking.GetRelativeMousePos(mousePos.x, mousePos.y);
			relPos = _mousePicking.SnapCoordinatesToGrid(relPos.x, relPos.y);

			for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
			{
				if (object->GetTransform()->GetPosition().x == relPos.x && object->GetTransform()->GetPosition().y == relPos.y)
					return;
			}

			GameObject* tempObj = new GameObject("Object" + std::to_string(objNum), { float(relPos.x), float(relPos.y), 1.0f }, { 2.5f, 2.5f }, 0.0f);
			objNum++;

			Component* component = new Appearance("Resources/Textures/" + _textureNames[_textureNum]);
			tempObj->AddComponent(component);
			_children.push_back(tempObj);
		}
		else if (me.GetType() == MouseEvent::EventType::WheelUp && selectedObj == -1) // Changes the texture that the next game object will be created with
		{
			_textureNum += 1;
			if (_textureNum == _textureNames.size())
				_textureNum = 0;
		}
		else if (me.GetType() == MouseEvent::EventType::WheelDown && selectedObj == -1)
		{
			_textureNum -= 1;
			if (_textureNum < 0)
				_textureNum = _textureNames.size() - 1;
		}
	}
#else
	for (GameObject* obj : _children)
		obj->Update(deltaTime);
#endif
}

void Scene::FixedUpdate(float timeStep)
{
	ObjectHandler::GetInstance().GetPhysicsWorld()->world->Step(timeStep, 8, 3);
	for (GameObject* obj : _children)
		obj->FixedUpdate(timeStep);
}

void Scene::Stop()
{
	for (GameObject* obj : _children)
		obj->Stop();
}

void Scene::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
	for (GameObject* object : _children)
		object->Render(context, constantBuffer, globalBuffer);
}
