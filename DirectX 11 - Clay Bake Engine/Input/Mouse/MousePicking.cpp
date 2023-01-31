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
	DirectX::XMINT2 returnPos = { posX + _width / 2, posY + _height / 2};

	returnPos.x /= snapScale;
	returnPos.y /= snapScale;
	returnPos.x *= snapScale;
	returnPos.y *= snapScale;
	returnPos.x -= _width / 2;
	returnPos.y -= _height / 2;
	returnPos.x += snapScale / 2;
	returnPos.y += snapScale / 2;

	return returnPos;
}