#pragma once
#include "../Transform.h"
#include "../../Physics/PhysicsInterface.h"
#include "../../Physics/PhysicsStructs.h"
#include "Component.h"

class Physics : public Component
{
public:
	Physics(PhysicsBody* body, PhysicsWorld* world);
	~Physics();

	void Update(float deltaTime);
	//GetPhysicInterface();
	PhysicsInterface GetPhysicInterface();


	b2World* CreatePhysicsWorld(float gravity);
	PhysicsWorld* GetWorld();

	Vector2 GetPosition();
	float GetAngleDegress();
	float GetAngleRadians();
	void SetAngleDegress(float angle);
	void SetAngleRadians(float angle);
	void SetTransform(Vector2 position, float angleOfRotation);
	PhysicsTransform GetTransform();
	b2Joint* CreateJoint(BindObjectsDef* jointDefinition);
	void SetLinearVelocity(Vector2 Velocity);
	Vector2 GetLinearVelocity();
	void SetAngularVelocity(float omega);
	void ApplyForceToPointOnObj(Vector2& force, Vector2& point, bool wake);
	void ApplyForceToObj(Vector2 force, bool wake);

	PhysicsBody* GetPhysicsBody() { return _objectPhysicsBody; }
	float GetDensity() { return _objectPhysicsBody->bodyDef.density; }
	float GetFriction() { return _objectPhysicsBody->bodyDef.friction; }

	HitBoxDefnintions CreateHitBox(Vector2 scale);
	void FixHitboxToBody(HitBoxDefnintions* hitbox, float density);
	void DeleteHitBox(b2Fixture* fixture);

	BodyDefinition SetCorrectBodyDef(PhysicsBody input, PhysicsBodyType type);
private:
	Transform* _pTransform;
	PhysicsInterface* _pPhysicsInterface;
	PhysicsWorld* _pWorld;
	PhysicsBody* _objectPhysicsBody;
};

