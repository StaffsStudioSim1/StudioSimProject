#pragma once

#include <DirectXMath.h>
#include "../../GameObjects/GameObject.h"

class MousePicking
{
public:
	void Initialise(int width, int height);
	GameObject* TestForObjectIntersection(int mouseX, int mouseY);
	DirectX::XMINT2 GetRelativeMousePos(int mouseX, int mouseY);
	DirectX::XMINT2 SnapCoordinatesToGrid(int posX, int posY);
	int GetSnapScale() const { return snapScale; }
private:
	int _height, _width;
	int snapScale = 18; // Edit this to change the snapping
};

