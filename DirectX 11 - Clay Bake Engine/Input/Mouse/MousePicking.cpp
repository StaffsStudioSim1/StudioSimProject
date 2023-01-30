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
	DirectX::BoundingBox objBox, mouseBox;
	DirectX::XMINT2 mousePos = GetRelativeMousePos(mouseX * 1, mouseY * 1);

	int objectNum = 0;
	for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
	{
		objBox.Center = { object->GetTransform()->GetPosition().x, object->GetTransform()->GetPosition().y, object->GetTransform()->GetDepthPos() };
		objBox.Extents = { object->GetTransform()->GetScale().x, object->GetTransform()->GetScale().y, 0.0f };

		mouseBox.Center = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), 0.0f };
		mouseBox.Extents = { 15.0f, 15.0f, 10.0f }; // The hitbox for the cursor

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
