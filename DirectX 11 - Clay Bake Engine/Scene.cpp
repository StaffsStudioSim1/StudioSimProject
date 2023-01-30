#include "Scene.h"
#include <fstream>
#include "ErrorLogger.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

Scene::Scene(std::string filePath)
{
	std::ifstream f(filePath);
	if (!f.good())
		ErrorLogger::Log("Unable to find scene file " + filePath);

	data = json::parse(f);

	std::string image = data["background_image"];

	for (json objectData : data["gameObjects"])
		_children.push_back(new GameObject(objectData));
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

void Scene::SaveScene()
{
	json scene;
	json gameObjects;

	for (GameObject* obj : _children)
	{
		gameObjects.push_back(obj->Write());
	}

	scene["gameObjects"] = gameObjects;
	scene["background_image"] = "";

	std::ofstream o("Resources/scene_demo.json");
	o << std::setw(4) << scene << std::endl;
	o.close();
}

void Scene::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
	for (GameObject* object : _children)
		object->Render(context, constantBuffer, globalBuffer);
}
