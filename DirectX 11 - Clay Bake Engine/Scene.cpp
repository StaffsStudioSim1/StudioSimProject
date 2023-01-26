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

	DirectX::XMINT2 mousePos = { InputManager::GetInstance().GetMouse().GetPosX(), InputManager::GetInstance().GetMouse().GetPosY() };

	if (InputManager::GetInstance().GetMouse().IsLeftDown())
	{
		selectedObj = _mousePicking.TestForObjectIntersection(mousePos.x, mousePos.y, selectedObj);
	}
	else if (InputManager::GetInstance().GetMouse().IsRightDown() && selectedObj != -1)
	{
		GameObject* object = ObjectHandler::GetInstance().GetAllObjects()[selectedObj];
		DirectX::XMFLOAT2 objectPos = object->GetTransform()->GetPosition();
		int snapScale = 10;
		int objXPos = static_cast<int>(objectPos.x / snapScale) * snapScale;
		int objYPos = static_cast<int>(objectPos.y / snapScale) * snapScale;
		object->GetTransform()->SetPosition(objXPos, objYPos);
		selectedObj = -1;
	}

	if (selectedObj != -1)
	{
		//DirectX::XMINT3 mouseInfo = _mousePicking.TestForObjectIntersection(InputManager::GetInstance().GetMouse().GetPosX(), InputManager::GetInstance().GetMouse().GetPosY());

		GameObject* object = ObjectHandler::GetInstance().GetAllObjects()[selectedObj];
		DirectX::XMINT2 relativeMousePos = _mousePicking.GetRelativeMousePos(mousePos.x, mousePos.y);
		object->GetTransform()->SetPosition(relativeMousePos.x, relativeMousePos.y);
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
