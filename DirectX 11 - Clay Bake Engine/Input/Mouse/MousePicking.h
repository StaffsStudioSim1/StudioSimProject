#pragma once

#include <DirectXMath.h>

class MousePicking
{
public:
	void Initialise(int width, int height);
	int TestForObjectIntersection(int mouseX, int mouseY, int currentObj);
	DirectX::XMINT2 GetRelativeMousePos(int mouseX, int mouseY);
private:
	int _height, _width;
};

