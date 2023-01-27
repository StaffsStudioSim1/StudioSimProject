#include "Scene.h"
#include <fstream>
#include "ErrorLogger.h"
#include "nlohmann/json.hpp"
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

	std::string image = data["background_image"];

	for (json objectData : data["gameObjects"])
		_children.push_back(new GameObject(objectData));

#if EDIT_MODE
	_mousePicking.Initialise(1280, 720);
#endif
}

Scene::~Scene()
{
	for (GameObject* obj : _children)
		delete obj;
	_children.clear();
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

	std::vector<int> mouseInputs = InputManager::GetInstance().PollMouseInput();
	DirectX::XMINT2 mousePos = { InputManager::GetInstance().GetMouse().GetPosX(), InputManager::GetInstance().GetMouse().GetPosY() };

	if (mouseInputs[0] && selectedObj == -1) // If an object is near the cursor select it
	{
		selectedObj = _mousePicking.TestForObjectIntersection(mousePos.x, mousePos.y, selectedObj);
	}
	else if (mouseInputs[1] && selectedObj != -1) // If an object is selected and the left click is released stop moving the object and deselect it
	{
		GameObject* object = ObjectHandler::GetInstance().GetAllObjects()[selectedObj];
		DirectX::XMFLOAT2 objectPos = object->GetTransform()->GetPosition();
		int snapScale = 10;
		int objXPos = static_cast<int>(objectPos.x / snapScale) * snapScale;
		int objYPos = static_cast<int>(objectPos.y / snapScale) * snapScale;
		object->GetTransform()->SetPosition(objXPos, objYPos);
		selectedObj = -1;
	}
	else if (selectedObj != -1) // Move the selected object to the cursor's position
	{
		GameObject* object = ObjectHandler::GetInstance().GetGameObject(selectedObj);
		DirectX::XMINT2 relativeMousePos = _mousePicking.GetRelativeMousePos(mousePos.x, mousePos.y);
		object->GetTransform()->SetPosition(relativeMousePos.x, relativeMousePos.y);

		if (mouseInputs[4])
		{
			//_children.erase(std::remove(ObjectHandler::GetInstance().GetAllObjects().begin(), ObjectHandler::GetInstance().GetAllObjects().end(), object), ObjectHandler::GetInstance().GetAllObjects().end());
			_children.erase(_children.begin() + selectedObj);
			ObjectHandler::GetInstance().Unregister(object);
			selectedObj = -1;
		}
	}
	else if (mouseInputs[2] && selectedObj == -1)
	{
		static int objNum = 0;
		GameObject* tempObj = new GameObject("Object" + std::to_string(objNum), { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f);
		objNum++;

		Component* component = new Appearance("Resources/Textures/Test.dds");
		tempObj->AddComponent(component);
		_children.push_back(tempObj);
	}
#endif

	for (GameObject* obj : _children)
		obj->Update(deltaTime);
}

void Scene::FixedUpdate(float timeStep)
{
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
