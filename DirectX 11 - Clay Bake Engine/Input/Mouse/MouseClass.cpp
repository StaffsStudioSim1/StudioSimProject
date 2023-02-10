#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y)
{
	this->_leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	this->_eventBuffer.push(me);
}

void MouseClass::OnLeftReleased(int x, int y)
{
	this->_leftIsDown = false;
	this->_eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(int x, int y)
{
	this->_rightIsDown = true;
	this->_eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseClass::OnRightReleased(int x, int y)
{
	this->_rightIsDown = false;
	this->_eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y)
{
	this->_mbuttonDown = true;
	this->_eventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseClass::OnMiddleReleased(int x, int y)
{
	this->_mbuttonDown = false;
	this->_eventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void MouseClass::OnWheelUp(int x, int y)
{
	this->_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y)
{
	this->_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
	this->_x = x;
	this->_y = y;
	this->_eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	this->_eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

bool MouseClass::IsLeftDown()
{
	return this->_leftIsDown;
}

bool MouseClass::IsMiddleDown()
{
	return this->_mbuttonDown;
}

bool MouseClass::IsRightDown()
{
	return this->_rightIsDown;
}

int MouseClass::GetPosX()
{
	return this->_x;
}

int MouseClass::GetPosY()
{
	return this->_y;
}

MousePointer MouseClass::GetPos()
{
	return{ this->_x, this->_y };
}

bool MouseClass::EventBufferIsEmpty()
{
	return this->_eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (this->_eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = this->_eventBuffer.front(); //Get first event from buffer
		this->_eventBuffer.pop(); //Remove first event from buffer
		return e;
	}
}
