#include "PhysicsInterface.h"



PhysicsInterface::PhysicsInterface(b2World* _world)
{
	//b2Vec2 gravity(0.0f, -9.81f);
	world = _world;
}

PhysicsInterface::~PhysicsInterface()
{
	world = nullptr;
}


b2World* PhysicsInterface::Getworld()
{
	return world;
}

b2Body* PhysicsInterface::CreateBody(b2BodyDef* bodyDefinition)
{
	return world->CreateBody(bodyDefinition);
}

void PhysicsInterface::DestroyBody(b2Body* body)
{
	world->DestroyBody(body);
}

b2Vec2 PhysicsInterface::GetBodyPosition(b2Body* body)
{
	return body->GetPosition();
}

float PhysicsInterface::GetAngle(b2Body* body)
{
	return  body->GetAngle();
}

float PhysicsInterface::GetAngleDegress(b2Body* body)
{
	return RAD2DEG(GetAngle(body));
}

float PhysicsInterface::GetAngleRadians(b2Body* body)
{
	return GetAngle(body);
}

void PhysicsInterface::SetAngleDegress(b2Body* body, float angle)
{
	body->SetTransform(GetBodyPosition(body), DEG2RAD(angle));
}

void PhysicsInterface::SetAngleRadians(b2Body* body, float angle)
{
	body->SetTransform(GetBodyPosition(body), angle);
}

void PhysicsInterface::SetTransform(b2Body* body, b2Vec2 position, float angleOfRotation)
{
	body->SetTransform(position, DEG2RAD(angleOfRotation));
}

b2Transform PhysicsInterface::GetTransform(b2Body* objectBody)
{
	return objectBody->GetTransform();
}

b2Joint* PhysicsInterface::Createjoint(b2JointDef* jointDefinition)
{
	return world->CreateJoint(jointDefinition);
}

void PhysicsInterface::SetLinearVelocity(b2Body* objectBody, b2Vec2* Velocity)
{
	objectBody->SetLinearVelocity(*Velocity);
}

b2Vec2* PhysicsInterface::GetLinearVelocity(b2Body* objectBody)
{
	b2Vec2 output = objectBody->GetLinearVelocity();
	return &output;
}

void PhysicsInterface::SetAngularVelocity(b2Body* objectBody, float omega)
{
	objectBody->SetAngularVelocity(omega);
}

void PhysicsInterface::ApplyForceToPointOnObj(b2Body* objbody, b2Vec2& force, b2Vec2& point, bool wake)
{
	objbody->ApplyForce(force, point, wake);
}

void PhysicsInterface::ApplyForceToObj(b2Body* objBody, b2Vec2 force, bool wake)
{
	objBody->ApplyForceToCenter(force, wake);

}

void PhysicsInterface::Update(float deltaTime /*, int VelocityIterations, int positionIterations*/) // noted out code is so dynamic functionality can be added easily if needed
{
	//world->Step(deltaTime, 8, 3);
	//world->Step(deltaTime, velocityIterations, positionIterations);

}

b2PolygonShape PhysicsInterface::CreateHitBox(b2Vec2 Halfscale)
{
	b2PolygonShape hitBox;
	hitBox.SetAsBox(Halfscale.x, Halfscale.y);
	return hitBox;
}

void PhysicsInterface::FixHitboxToBody(b2Body* body, b2PolygonShape* hitbox, float density)
{
	body->CreateFixture(hitbox, density);
}

void PhysicsInterface::DeleteHitBox(b2Body* body, b2Fixture* fixture)
{
	body->DestroyFixture(fixture);
}

void PhysicsInterface::DestroyJoint(b2Joint* joint)
{
	world->DestroyJoint(joint);
}