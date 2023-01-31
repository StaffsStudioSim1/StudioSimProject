#include "MousePicking.h"
#include "../../GameObjects/ObjectHandler.h"

#include <DirectXCollision.h>

void MousePicking::Initialise(int width, int height) // Currently only being used to fix the maximised window issue
{
	_width = width;
	_height = height;
}

int MousePicking::TestForObjectIntersection(int mouseX, int mouseY, int currentObj)
{
	DirectX::BoundingBox objBox;
	DirectX::BoundingSphere mouseSphere;
	DirectX::XMINT2 mousePos = GetRelativeMousePos(mouseX, mouseY);

	int objectNum = 0;
	for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
	{
		objBox.Center = { object->GetTransform()->GetPosition().x, object->GetTransform()->GetPosition().y, object->GetTransform()->GetDepthPos() };
		objBox.Extents = { object->GetTransform()->GetScale().x, object->GetTransform()->GetScale().y, 0.0f };
		
		mouseSphere.Center = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), 0.0f };
		mouseSphere.Radius = 20.0f; // Cursor picking size

		if (mouseSphere.Intersects(objBox))
		{
			return objectNum;
		}
		objectNum++;
	}

	return -1;
}

DirectX::XMINT2 MousePicking::GetRelativeMousePos(int mouseX, int mouseY)
{
	int width = 0;
	int height = 0;

	RECT rc;
	GetWindowRect(GetActiveWindow(), &rc);
	width = (rc.right - rc.left) - 16; // Take away values due to the window adjustment function
	height = (rc.bottom - rc.top) - 39;

	WINDOWPLACEMENT wp;
	GetWindowPlacement(GetActiveWindow(), &wp);
	if (wp.flags == 2) // Hopefully temp but mostly fixes the mouse picking issue when the window is maximised
	{
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(GetActiveWindow(), MONITOR_DEFAULTTONEAREST), &mi);
		float monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
		float monitorHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;
		width = _width;
		height = _height;

		float diff = height / monitorHeight;
		mouseX *= (width / monitorWidth);
		mouseY *= (height / (monitorHeight - 39));
	}

	return { mouseX - (width / 2), -(mouseY - (height / 2)) };
}

DirectX::XMINT2 MousePicking::SnapCoordinatesToGrid(int posX, int posY)
{
	int snapScale = 60; // Edit this to change the snapping
	DirectX::XMINT2 returnPos = { posX, posY };

	int roundedX = (returnPos.x / snapScale) * snapScale;
	int roundedY = (returnPos.y / snapScale) * snapScale;
	int xDiff = returnPos.x - roundedX;
	int yDiff = returnPos.y - roundedY;

	if (xDiff >= snapScale / 2)
		returnPos.x = roundedX + snapScale;
	else if (xDiff <= -snapScale / 2)
		returnPos.x = roundedX - snapScale;
	else
		returnPos.x = roundedX;

	if (yDiff >= snapScale / 2)
		returnPos.y = roundedY + snapScale;
	else if (yDiff <= -snapScale / 2)
		returnPos.y = roundedY - snapScale;
	else
		returnPos.y = roundedY;

	return returnPos;
}
