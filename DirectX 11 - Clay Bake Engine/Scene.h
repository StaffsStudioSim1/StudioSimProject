#pragma once
#include <vector>
#include <string>
#include "GameObject.h"
class Scene
{
public:
	Scene(std::string filePath);
	~Scene();

	void Start();
	void Update();
	void Stop();
private:
	std::vector<GameObject*> _children;
};

