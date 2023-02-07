#pragma once

#include "../../GameObjects/GameObject.h"

class MousePicking
{
public:
	void Initialise(int width, int height);
	GameObject* TestForObjectIntersection(Vector2 mousePos);
	Vector2 GetRelativeMousePos(Vector2 mousePos);
	Vector2 SnapCoordinatesToGrid(Vector2 worldPos);
	Vector2 WorldToGrid(Vector2 worldPos);
	Vector2 GridToWorld(Vector2 gridPos);
	int GetSnapScale() const { return snapScale; }
private:
	int _height, _width;
	int snapScale = 18; // Edit this to change the snapping
};

