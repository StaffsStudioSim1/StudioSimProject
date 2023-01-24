#include "Physics.h"

Physics::Physics(Transform* transform, PhysicsWorld* _world) // Initilaise transform variables
{
	_pTransform = transform;
	_pWolrd = _world;
	_pPhysicsInterface = new PhysicsInterface(&_pWolrd->_world);
}

Physics::~Physics()
{
}

void Physics::Update(float deltaTime) // Update physics
{
	_pPhysicsInterface->Update(deltaTime);

}

PhysicsInterface Physics::GetPhysicInterface()
{
	return *_pPhysicsInterface;
}

PhysicsWorld* Physics::Getworld()
{
	return _pWolrd;
}

PhysicsBody* Physics::CreateBody(BodyDefinition* _bodyDefinition)
{
	PhysicsBody output;
	output._body = _pPhysicsInterface->CreateBody(&(_bodyDefinition->_bodyDef)); // _pPhysicsInterface->CreateBody(&_bodyDefinition->_bodyDef);
	return &output;
}

void Physics::DestroyBody(PhysicsBody* body)
{
	_pPhysicsInterface->DestroyBody(body->_body);
}

Vector2 Physics::GetPosition(PhysicsBody* body)
{
	Vector2 output(body->_body->GetPosition().x, body->_body->GetPosition().y);
	return output;
}

//float Physics::GetAngle(PhysicsBody* body)
//{
//	
//}

float Physics::GetAngleDegress(PhysicsBody* body)
{
	float output = _pPhysicsInterface->GetAngleDegress(body->_body);
	return output;
}

float Physics::GetAngleRadians(PhysicsBody* body)
{
	float output = _pPhysicsInterface->GetAngleRadians(body->_body);
	return output;
}

void Physics::SetAngleDegress(PhysicsBody* body, float angle)
{
	_pPhysicsInterface->SetAngleDegress(body->_body, angle);
}

void Physics::SetAngleRadians(PhysicsBody* body, float angle)
{
	_pPhysicsInterface->SetAngleRadians(body->_body, angle);
}

void Physics::SetTransform(PhysicsBody* body, Vector2 position, float angleOfRotation)
{
	body->_body->SetTransform(b2Vec2(position.x, position.y), angleOfRotation);
}

PhysicsTransform Physics::GetTransform(PhysicsBody* objectBody)
{
	PhysicsTransform output;
	output._Transformation = objectBody->_body->GetTransform();
	return output;
}

b2Joint* Physics::Createjoint(BindObjectsDef* jointDefinition)
{
	return _pWolrd->_world.CreateJoint(&jointDefinition->_jointdefinition);
}

void Physics::SetLinearVelocity(PhysicsBody* objectBody, Vector2* Velocity)
{
	b2Vec2 input;
	input.x = Velocity->x; input.y = Velocity->y;
	_pPhysicsInterface->SetLinearVelocity(objectBody->_body, &input);
}

Vector2* Physics::GetLinearVelocity(PhysicsBody* objectBody)
{
	Vector2 output;
	b2Vec2* swapper = _pPhysicsInterface->GetLinearVelocity(objectBody->_body);
	output.x = swapper->x; output.y = swapper->y;
	return &output;
}

void Physics::SetAngularVelocity(PhysicsBody* objectBody, float omega)
{
	objectBody->_body->SetAngularVelocity(omega);
}

void Physics::ApplyForceToPointOnObj(PhysicsBody* objbody, Vector2& force, Vector2& point, bool wake)
{
	b2Vec2 b2ForceIn; b2ForceIn.x = force.x; b2ForceIn.y = force.y;
	b2Vec2 b2PointIn; b2PointIn.x = point.x; b2PointIn.y = force.y;
	objbody->_body->ApplyForce(b2ForceIn, b2PointIn, wake);
}

void Physics::ApplyForceToObj(PhysicsBody* objBody, Vector2 force, bool wake)
{
	b2Vec2 b2ForceIn; b2ForceIn.x = force.x; b2ForceIn.y = force.y;
	_pPhysicsInterface->ApplyForceToObj(objBody->_body, b2ForceIn, wake);
}

HitBoxDefnintions Physics::CreateHitBox(Vector2 scale)
{
	b2Vec2 halfScale; halfScale.x = scale.x / 2; halfScale.y = scale.y / 2;

	HitBoxDefnintions output; output._HitBox = _pPhysicsInterface->CreateHitBox(halfScale);
	return output;
}

void Physics::FixHitboxToBody(PhysicsBody* body, HitBoxDefnintions* hitbox, float density)
{
	_pPhysicsInterface->FixHitboxToBody(body->_body, &hitbox->_HitBox, density);
}

void Physics::DeleteHitBox(PhysicsBody* body, b2Fixture* fixture)
{
	_pPhysicsInterface->DeleteHitBox(body->_body, fixture);

}

void Physics::UpdateObject(PhysicsBody* body)
{
}
