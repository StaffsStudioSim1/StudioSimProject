#pragma once
#include <vector>
#include <string>
#include "GameObjects/GameObject.h"
class Scene
{
public:
	Scene(std::string filePath);
	~Scene();

	void Start();
	void Update(float deltaTime);
	void FixedUpdate(float timeStep);
	void Stop();
private:
	std::vector<GameObject*> _children;
};

