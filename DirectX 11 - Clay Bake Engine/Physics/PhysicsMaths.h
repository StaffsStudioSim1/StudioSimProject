#pragma once
#include <d3d11.h>
//#include <math.h>
#include<DirectXMath.h>
#include"../PhysicsStructs.h"
#include"Vectors.h"
#define FORCE_0F_GRAVITY_PER_KG = 9.80665

class PhysicsMaths
{
public:
	PhysicsMaths();
	~PhysicsMaths();

	void InitGravity();
	void CustomGravityValue(float NewGravity);
	void InvertGravity();
//	GravityDirectinalProperties GetGravityDirection() { return CurrentGravityDirection; }

	void SetGravityActive();
	bool GravityisActive() { return GravityActive; }

	DirectinalGravity GetGravityValue() { return GravityValue; }

	void SetObjectWeight(float Weight);
	float GetObjectWeight();
//	int GetObjectIntegerWeight();

	void CreateMass();
	float GetMass();

	bool AABBvsAABB(AxisAlignedBoundingBox a, AxisAlignedBoundingBox b);// axis bound based box collision 



protected:

private:
	bool GravityActive;
	float CurrentGravityValueX;
	float CurrentGravityValueY;
	DirectinalGravity GravityValue;
//	GravityDirectinalProperties CurrentGravityDirection;


};
