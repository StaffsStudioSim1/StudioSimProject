#ifndef _H_PHYSICS_SYSTEM_
#define _H_PHYSICS_SYSTEM_

#include "Rigidbody.h"
#include "Vectors.h"
#include "FixedFunctionPrimitives.h"


#define FORCE_OF_GRAVITY float(9.80665f)

struct objectData 
{
	ColliderType ColisionBoxType;
	Vector2 Velocity;
};

enum ColliderType
{
	_None,
	_Line,
	_Rectangle,
	_OrientedRectangle,
	_Cirle,
	_ContaningCircle,
	_null

};


class PhysicsSystem
{
protected: 
	std::vector<Rigidbody*> bodies;

	std::vector<Rigidbody*> colliders1;
	std::vector<Rigidbody*> colliders2;
//	std::vector<CollisionManifold> results;

	objectData* obj1Data;
	objectData* obj2Data;

public:
	PhysicsSystem();

	//float LinearProjectionPercent; // [0.2 to 0.8], Smaller = less jitter / more penetration
	//float PenetrationSlack; // [0.01 to 0.1],  Samller = more accurate
	//int ImpulseIteration;

	// need to add func to switch between types of hitboxes for the 2d collisions 
	
	
	void Update(float deltaTime);
	void Render();

	void AddRigidbody(Rigidbody* body);
	void AddGravity();

	void ClearRigidbodys();
	void ClearGravity();

	void ApplyForce(Vector2* froce);

	bool CheckForCollison(objectData* objData1, objectData* objData2);


	bool IsObjectStatic();

};

#endif // !_H_PHYSICS_SYSTEM_