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

typedef struct vec2 
{
	union 
	{
		struct
		{
			float x;
			float y;
		};
		float asArray[2];
	};

	float& operator[](int i)
	{
		return asArray[i];
	}

} vec2;