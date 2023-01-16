#pragma once



enum GravityDirectinalProperties
{
	DEFAULT,
	UPSIDEDOWN,
	TORIGHT,
	TOLEFT,
	DISABLED
};

struct Momentum
{
	float SpeedX;
	float SpeedY;

	float WeightAsKg;

	float TotalSpeed = SpeedX + SpeedY;
	float KilogramMeterPerSecond = TotalSpeed * WeightAsKg;
};

struct DirectinalGravity
{
	float SpeedX;
	float SpeedY;
};


//struct AxisAlignedBoundingBox
//{
//	Vector2 min;
//	Vector2 max;
//};