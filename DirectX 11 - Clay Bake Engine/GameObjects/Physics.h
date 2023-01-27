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
	 PhysicsInterface GetPhysicInterface();

	
	
	 PhysicsWorld* Getworld();

	 PhysicsBody* CreateBody(BodyDefinition* _bodyDefinition);
	 void DestroyBody(PhysicsBody* body);
	 Vector2 GetPosition(PhysicsBody* body);
	//float GetAngle(PhysicsBody* body);
	float GetAngleDegress(PhysicsBody* body);
	float GetAngleRadians(PhysicsBody* body);
	void SetAngleDegress(PhysicsBody* body, float angle);
	void SetAngleRadians(PhysicsBody* body, float angle);
	void SetTransform(PhysicsBody* body, Vector2 position, float angleOfRotation);
	PhysicsTransform GetTransform(PhysicsBody* objectBody);
	b2Joint* Createjoint(BindObjectsDef* jointDefinition);
	void SetLinearVelocity(PhysicsBody* objectBody, Vector2* Velocity);
	Vector2* GetLinearVelocity(PhysicsBody* objectBody);
	void SetAngularVelocity(PhysicsBody* objectBody, float omega);
	void ApplyForceToPointOnObj(PhysicsBody* objbody, Vector2& force, Vector2& point, bool wake);
	void ApplyForceToObj(PhysicsBody* objBody, Vector2 force, bool wake);

	HitBoxDefnintions CreateHitBox(Vector2 scale);
	void FixHitboxToBody(PhysicsBody* body, HitBoxDefnintions* hitbox, float density);
	void DeleteHitBox(PhysicsBody* body, b2Fixture* fixture);
	void UpdateObject(PhysicsBody* body);

private:
	 Transform* _pTransform;
	 PhysicsInterface* _pPhysicsInterface;
	 PhysicsWorld* _pWolrd;
};

