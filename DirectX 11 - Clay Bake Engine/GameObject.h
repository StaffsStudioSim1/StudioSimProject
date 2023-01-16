#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class GameObject
{
public:
	GameObject();
	GameObject(json json);
	~GameObject();

	void Start();
	void Update();
	void Stop();
};

