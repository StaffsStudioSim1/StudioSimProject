#pragma once
#include "Transform.h"
#include "../Physics/PhysicsInterface.h"
#include "../PhysicsStructs.h"

class Physics
{
public:
	Physics(Transform* transform, PhysicsWorld* _world);
	~Physics();

	void Update(float deltaTime);
	//GetPhysicInterface();
	const PhysicsInterface GetPhysicInterface();

	
	
	const PhysicsWorld* Getworld();

	 PhysicsBody* CreateBody(BodyDefinition* _bodyDefinition);
	const void DestroyBody(PhysicsBody* body);
	const Vector2 GetPosition(PhysicsBody* body);
	//float GetAngle(PhysicsBody* body);
	const float GetAngleDegress(PhysicsBody* body);
	const float GetAngleRadians(PhysicsBody* body);
	const void SetAngleDegress(PhysicsBody* body, float angle);
	const void SetAngleRadians(PhysicsBody* body, float angle);
	const void SetTransform(PhysicsBody* body, Vector2 position, float angleOfRotation);
	const PhysicsTransform GetTransform(PhysicsBody* objectBody);
	const b2Joint* Createjoint(BindObjectsDef* jointDefinition);
	const void SetLinearVelocity(PhysicsBody* objectBody, Vector2* Velocity);
	Vector2* GetLinearVelocity(PhysicsBody* objectBody);
	const void SetAngularVelocity(PhysicsBody* objectBody, float omega);
	const void ApplyForceToPointOnObj(PhysicsBody* objbody, Vector2& force, Vector2& point, bool wake);
	const void ApplyForceToObj(PhysicsBody* objBody, Vector2 force, bool wake);

	const HitBoxDefnintions CreateHitBox(Vector2 scale);
	const void FixHitboxToBody(PhysicsBody* body, HitBoxDefnintions* hitbox, float density);
	const void DeleteHitBox(PhysicsBody* body, b2Fixture* fixture);
	const void UpdateObject(PhysicsBody* body);

private:
	 Transform* _pTransform;
	 PhysicsInterface* _pPhysicsInterface;
	 PhysicsWorld* _pWolrd;
};

