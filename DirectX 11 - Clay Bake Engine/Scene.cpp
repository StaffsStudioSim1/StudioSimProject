#include "Scene.h"
#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

Scene::Scene(std::string filePath)
{
	std::ifstream f(filePath);
	json data = json::parse(f);

	for (json objectData : data["gameObjects"])
	{
		GameObject* gameObject = new GameObject();
		_children.push_back(gameObject);
	}
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

void Scene::Update()
{
	for (GameObject* obj : _children)
		obj->Update();
}

void Scene::Stop()
{
	for (GameObject* obj : _children)
		obj->Stop();
}
