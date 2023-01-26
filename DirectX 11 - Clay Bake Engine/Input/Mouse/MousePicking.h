#pragma once

#include <DirectXMath.h>

class MousePicking
{
public:
	void Initialise(int width, int height);
	DirectX::XMINT3 TestForObjectIntersection(int mouseX, int mouseY);
private:
	int _height, _width;
};

