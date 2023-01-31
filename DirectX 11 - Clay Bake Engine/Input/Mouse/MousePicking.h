#pragma once

#include <DirectXMath.h>

class MousePicking
{
public:
	void Initialise(int width, int height);
	int TestForObjectIntersection(int mouseX, int mouseY, int currentObj);
	DirectX::XMINT2 GetRelativeMousePos(int mouseX, int mouseY);
	DirectX::XMINT2 SnapCoordinatesToGrid(int posX, int posY);
	int GetSnapScale() const { return snapScale; }
private:
	int _height, _width;
	int snapScale = 50; // Edit this to change the snapping
};

