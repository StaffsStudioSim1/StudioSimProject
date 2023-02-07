#pragma once
#include "Transform.h"
#include "../PhysicsStructs.h"
#include "Component.h"
#include <vector>
#include <DirectXCollision.h>

class Physics : public Component
{
public:
	Physics(PhysicsBody* body, Vector2 inputGravity);
	~Physics();

	json Write();
	void Stop();

	void Update();

	PhysicsBody* GetPhysicsBody();
	Vector2 GetPosition();
	float GetAngleDegress();
	float GetAngleRadians();
	void SetAngleDegress(float angle);
	void SetAngleRadians(float angle);
	void SetTransform(Vector2 position, float angleOfRotation);
	Vector2 GetTransform();
	void SetLinearVelocity(Vector2 Velocity);
	Vector2 GetLinearVelocity();
	void ApplyForceToObj(Vector2 force);
	void ApplyImpulseForceToObj(Vector2 force);

	PhysicsBodyType GetBodyType() { return _objectPhysicsBody->hitbox.bodyType; }
	float GetDensity() { return _objectPhysicsBody->bodyDef.density; }
	float GetFriction() { return _objectPhysicsBody->bodyDef.friction; }

	HitBoxDefnintions CreateHitBox(Vector2 scale);
	std::vector<int> GetObjectsInAreaByID(Vector2 position, Vector2 areaScale);
	int GetNumberOfObjectsInArea(Vector2 position, Vector2 areaScale);

	bool IsObjectCollidingwith(PhysicsBody input);
	std::vector<int> GetObjectsCollisionsByID(Vector2 position, HitBoxDefnintions hitBoxDef);
	
	void ApplyGravityForceForUpdate();
	void updateBodyForces();

	int GetNumberOfCollisonsWithBody(Vector2 position, HitBoxDefnintions hitboxdef);
	void PhysicsStaticCollision(int objectID);
	void PhysicsDynamicCollision(int objectID);

	void UpdateGravity(Vector2 inputGravity) { _pGravityAsForce = inputGravity; }
	Vector2 GetGravityValue() { return _pGravityAsForce; }


	void FixedUpdate(float timeStep);
	Vector2 GetSteppedGravityForce();
private:
	Transform* _pTransform;

	PhysicsWorld* _pWorld;
	PhysicsBody* _objectPhysicsBody;
	Vector2 _pGravityAsForce;

};

