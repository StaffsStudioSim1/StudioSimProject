#include "MousePicking.h"
#include "../../GameObjects/ObjectHandler.h"
#include "../../Graphics/Graphics.h"

void MousePicking::Initialise(int width, int height) // Currently only being used to fix the maximised window issue
{
	_width = width;
	_height = height;
}

GameObject* MousePicking::TestForObjectIntersection(Vector2 mousePos)
{
	Vector2 gridPos = SnapCoordinatesToGrid(GetRelativeMousePos(mousePos));

	for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
	{
		if (object->GetName() == JSON_SCENE_BACKGROUND)
			continue;

		if (object->GetTransform()->GetPosition() == gridPos)
			return object;
	}

	return nullptr;
}

Vector2 MousePicking::GetRelativeMousePos(Vector2 mousePos)
{
	float scaleX = _width / INTERNAL_RESOLUTION_X;
	float scaleY = _height / INTERNAL_RESOLUTION_Y;
	return Vector2((mousePos.x - (_width / 2)) / scaleX, -(mousePos.y - (_height / 2)) / scaleY);
}

Vector2 MousePicking::SnapCoordinatesToGrid(Vector2 worldPos)
{
	return GridToWorld(WorldToGrid(worldPos));
}

Vector2 MousePicking::WorldToGrid(Vector2 worldPos)
{
	int x = (worldPos.x + INTERNAL_RESOLUTION_X / 2) / snapScale;
	int y = (worldPos.y + INTERNAL_RESOLUTION_Y / 2) / snapScale;
	return Vector2(x, y);
}

Vector2 MousePicking::GridToWorld(Vector2 gridPos)
{
	gridPos *= snapScale;
	gridPos -= Vector2(_width / 2, _height / 2);
	gridPos += Vector2(snapScale / 2, snapScale / 2);
	gridPos += Vector2(INTERNAL_RESOLUTION_X / 2, INTERNAL_RESOLUTION_Y / 2);
	return gridPos;
}
