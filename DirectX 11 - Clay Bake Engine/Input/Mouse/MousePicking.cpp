#include "MousePicking.h"
#include "../../GameObjects/ObjectHandler.h"

#include <DirectXCollision.h>

void MousePicking::Initialise(int width, int height)
{
	_width = width;
	_height = height;
}

DirectX::XMINT3 MousePicking::TestForObjectIntersection(int mouseX, int mouseY)
{
	DirectX::XMINT3 returnInfo = { 0, 0, -1 };

	DirectX::BoundingBox objBox, mouseBox;
	DirectX::XMFLOAT2 mousePos = { mouseX - (_width / 2.0f), mouseY - (_height / 2.0f) };

	int objectNum = 0;
	for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
	{
		objBox.Center = { object->GetTransform()->GetPosition().x, object->GetTransform()->GetPosition().y, object->GetTransform()->GetDepthPos() };
		objBox.Extents = { object->GetTransform()->GetScale().x, object->GetTransform()->GetScale().y, 0.0f };

		mouseBox.Center = { mousePos.x, mousePos.y, 0.0f };
		mouseBox.Extents = { 5.0f, 5.0f, 2.0f };

		if (mouseBox.Intersects(objBox))
		{
			if (mouseBox.Center.x - objBox.Center.x < -50.0f)
				returnInfo.x = -1;
			else if (mouseBox.Center.x - objBox.Center.x > 50.0f)
				returnInfo.x = 1;

			if (mouseBox.Center.y + objBox.Center.y < -50.0f)
				returnInfo.y = 1;
			else if (mouseBox.Center.y + objBox.Center.y > 50.0f)
				returnInfo.y = -1;

			returnInfo.z = objectNum;
			return returnInfo;
		}

		objectNum++;
	}

	return { 0, 0, -1 };
}
