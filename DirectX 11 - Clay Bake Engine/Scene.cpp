#include "Scene.h"
#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

Scene::Scene(std::string filePath)
{
	/*std::ifstream f(filePath);
	json data = json::parse(f);

	for (json objectData : data["gameObjects"])
	{
		GameObject* gameObject = new GameObject();
		_children.push_back(gameObject);
	}*/

	GameObject* test1 = new GameObject("ObjectTest", { 0.0f, 0.0f, 1.0f }, { 2.0f, 2.0f }, 0.0f);
	test1->AddComponent(new Appearance("Test", { 1.0f, 1.0f, 0.0f, 0.0f }));
	GameObject* test2 = new GameObject("ObjectTest2", { 0.0f, 0.0f, 0.5f }, { 1.5f, 1.5f }, 3.141f);
	test2->AddComponent(new Appearance("Test", { 1.0f, 1.0f, 0.0f, 0.0f }));
	GameObject* test3 = new GameObject("ObjectTest3", { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f);
	test3->AddComponent(new Appearance("Test", { 1.0f, 1.0f, 0.0f, 0.0f }));

	_children.push_back(test1);
	_children.push_back(test2);
	_children.push_back(test3);
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

void Scene::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, ConstantBuffer& constantBuffer, Microsoft::WRL::ComPtr <ID3D11Buffer> globalBuffer)
{
	for (GameObject* object : _children)
		object->Render(context, constantBuffer, globalBuffer);
}
