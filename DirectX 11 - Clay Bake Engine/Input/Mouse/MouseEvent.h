#pragma once
struct MousePointer
{
	int X;
	int Y;
};

class MouseEvent
{
public:
	enum EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid
	};
	MouseEvent();
	MouseEvent(const EventType type, const int x, const int y);
	bool IsValid() const;
	EventType GetType() const;
	MousePointer GetPos() const;
	int GetPosX() const; 
	int GetPosY() const;

protected:
	
private:
	EventType type;
	int x;
	int y;


};