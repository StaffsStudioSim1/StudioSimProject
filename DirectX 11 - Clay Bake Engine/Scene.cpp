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

	std::string imagePath = data[JSON_SCENE_BACKGROUND];
	_backgroundImage = new GameObject((std::string) JSON_SCENE_BACKGROUND);
	_backgroundImage->GetTransform()->SetDepthPos(1.0f);
	_backgroundImage->AddComponent(new Appearance(imagePath));

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

	delete _backgroundImage;
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
	static GameObject* selectedObj = nullptr;
	static DirectX::XMINT2 startingPos = { 0, 0 };

	MouseClass* mouse = InputManager::GetInstance().GetMouse();
	DirectX::XMINT2 mousePos = { mouse->GetPosX(), mouse->GetPosY() };

	while (!mouse->EventBufferIsEmpty()) // Handles moving, creating and deleting objects with the mouse in edit mode
	{
		MouseEvent me = mouse->ReadEvent();
		// Pickup Tile
		if (me.GetType() == MouseEvent::EventType::LPress && !selectedObj) // Probably can be changed to a switch statement
		{
			selectedObj = _mousePicking.TestForObjectIntersection(mousePos.x, mousePos.y);
			if (selectedObj)
			{
				Vector2 pos = selectedObj->GetTransform()->GetPosition();
				startingPos = { (int)pos.x, (int)pos.y };
			}
		}
		// Drop tile at position
		else if (me.GetType() == MouseEvent::EventType::LRelease && selectedObj)
		{
			Vector2 objectPos = selectedObj->GetTransform()->GetPosition();
			DirectX::XMINT2 snapPos = _mousePicking.SnapCoordinatesToGrid(objectPos.x, objectPos.y);

			for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
			{
				if (object != selectedObj && object->GetTransform()->GetPosition().x == snapPos.x && object->GetTransform()->GetPosition().y == snapPos.y)
				{
					selectedObj->GetTransform()->SetPosition(startingPos.x, startingPos.y);
					selectedObj = nullptr;
					return;
				}
			}

			selectedObj->GetTransform()->SetPosition(snapPos.x, snapPos.y);
			selectedObj = nullptr;
		}
		// If an object is following the cursor
		else if (selectedObj)
		{
			DirectX::XMINT2 relativeMousePos = _mousePicking.GetRelativeMousePos(mousePos.x, mousePos.y);
			selectedObj->GetTransform()->SetPosition(relativeMousePos.x, relativeMousePos.y);

			if (me.GetType() == MouseEvent::EventType::MPress) // Remove the currently selected game object
			{
				_children.erase(std::remove(_children.begin(), _children.end(), selectedObj), _children.end());
				delete selectedObj;
				selectedObj = nullptr;
			}
		}
		// Create a new tile
		else if (me.GetType() == MouseEvent::EventType::RPress && !selectedObj) // Creates a new game object
		{
			static int objNum = 0;
			DirectX::XMINT2 relPos = _mousePicking.GetRelativeMousePos(mousePos.x, mousePos.y);
			relPos = _mousePicking.SnapCoordinatesToGrid(relPos.x, relPos.y);

			for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
			{
				if (object->GetTransform()->GetPosition().x == relPos.x && object->GetTransform()->GetPosition().y == relPos.y)
					return;
			}

			GameObject* tempObj = new GameObject("Object" + std::to_string(objNum), { float(relPos.x), float(relPos.y), 0.0f }, { 1.0f, 1.0f }, 0.0f);
			objNum++;

			Component* component = new Appearance("Resources/Textures/" + _textureNames[_textureNum]);
			tempObj->AddComponent(component);
			_children.push_back(tempObj);
		}
		// Change tile type to be made
		else if (me.GetType() == MouseEvent::EventType::WheelUp && !selectedObj) // Changes the texture that the next game object will be created with
		{
			_textureNum += 1;
			if (_textureNum == _textureNames.size())
				_textureNum = 0;
		}
		// Change tile type to be made
		else if (me.GetType() == MouseEvent::EventType::WheelDown && !selectedObj)
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
	_backgroundImage->Render(context, constantBuffer, globalBuffer);
}
