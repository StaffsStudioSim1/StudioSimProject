#pragma once
#include <d3d11.h>
#include<DirectXMath.h>
#include"PhysicsStructs.h"
#define FORCE_0F_GRAVITY_PER_KG = 9.80665

class PhysicsMaths
{
public:
	PhysicsMaths();
	~PhysicsMaths();

	void InitGravity();
	void CustomGravityValue(float NewGravity);
	void InvertGravity();
	GravityDirectinalProperties GetGravityDirection() { return CurrentGravityDirection; }

	void SetGravityActive();
	void SetGravityTrue();
	bool GravityisActive() { return GravityActive; }
	bool GravityIsTrue() { return GravityActive; }

	DirectinalGravity GetGravityValue() { return GravityValue; }

	void SetObjectWeight(float Weight);
	float GetObjectWeight();
	int GetObjectIntegerWeigt();





protected:

private:
	bool GravityActive;
	float CurrentGravityValueX;
	float CurrentGravityValueY;
	DirectinalGravity GravityValue;
	GravityDirectinalProperties CurrentGravityDirection;


};
