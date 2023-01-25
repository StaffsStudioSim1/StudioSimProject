#pragma once
#include <Box2D/box2d.h>
#include <iostream>
#include <box2d/b2_math.h>
//#include "PhysicsStructs.h"

class PhysicsInterface
{
public:
	PhysicsInterface(b2World* _world);
	~PhysicsInterface();
	float Deg2Rad(float degress);
	float Rad2Deg(float radians);
	float CorrectDegrees(float degrees);
	b2World* Getworld();

	b2Body* CreateBody(b2BodyDef* bodyDefinition);
	void DestroyBody(b2Body* body);
	b2Vec2 GetBodyPosition(b2Body* body);
	float GetAngle(b2Body* body);
	float GetAngleDegress(b2Body* body);
	float GetAngleRadians(b2Body* body);
	void SetAngleDegress(b2Body* body, float angle);
	void SetAngleRadians(b2Body* body, float angle);
	void SetTransform(b2Body* body, b2Vec2 position, float angleOfRotation);
	b2Transform GetTransform(b2Body* objectBody);
	b2Joint* Createjoint(b2JointDef* jointDefinition);
	void DestroyJoint(b2Joint* joint);
	void SetLinearVelocity(b2Body* objectBody, b2Vec2* Velocity);
	b2Vec2* GetLinearVelocity(b2Body* objectBody);
	void SetAngularVelocity(b2Body* objectBody, float omega);
	void ApplyForceToPointOnObj(b2Body* objbody, b2Vec2& force, b2Vec2& point, bool wake);
	void ApplyForceToObj(b2Body* objBody, b2Vec2 force, bool wake);
	void Update(float deltaTime);


	b2PolygonShape CreateHitBox(b2Vec2 Halfscale);
	void FixHitboxToBody(b2Body* body, b2PolygonShape* hitbox, float density);
	void DeleteHitBox(b2Body* body, b2Fixture* fixture);



private:
	b2World* world;
	//Vec2 gravity = b2Vec2(0.0f, 9.81f);
};

