#include "MousePicking.h"
#include "../../GameObjects/ObjectHandler.h"

#include <DirectXCollision.h>

void MousePicking::Initialise(int width, int height)
{
	_width = width;
	_height = height;
}

int MousePicking::TestForObjectIntersection(int mouseX, int mouseY, int currentObj)
{
	DirectX::XMINT3 returnInfo = { 0, 0, -1 };

	DirectX::BoundingBox objBox, mouseBox;
	DirectX::XMINT2 mousePos = GetRelativeMousePos(mouseX * 1, mouseY * 1);

	int objectNum = 0;
	for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
	{
		//if (objectNum == currentObj)
		//	continue;

		objBox.Center = { object->GetTransform()->GetPosition().x, object->GetTransform()->GetPosition().y, object->GetTransform()->GetDepthPos() };
		objBox.Extents = { object->GetTransform()->GetScale().x, object->GetTransform()->GetScale().y, 0.0f };

		mouseBox.Center = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), 0.0f };
		mouseBox.Extents = { 25.0f, 25.0f, 10.0f };

		if (mouseBox.Intersects(objBox))
		{
			return objectNum;
		}

		objectNum++;
	}

	return -1;
}

DirectX::XMINT2 MousePicking::GetRelativeMousePos(int mouseX, int mouseY)
{
	return { int((float(mouseX * 1.01f))) - (_width / 2), -(int(float(mouseY * 1.05f)) - (_height / 2)) };
}
