#pragma once

#include "../Transform.h"

class Physics
{
public:
	Physics(Transform* transform);
	~Physics() {}

	void Update();
private:
	Transform* _pTransform;
};

