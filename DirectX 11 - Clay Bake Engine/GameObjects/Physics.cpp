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

const PhysicsInterface Physics::GetPhysicInterface()
{
	return *_pPhysicsInterface;
}

const PhysicsWorld* Physics::Getworld()
{
	return _pWolrd;
}

PhysicsBody* Physics::CreateBody(BodyDefinition* _bodyDefinition)
{
	PhysicsBody output;
	output._body = _pPhysicsInterface->CreateBody(&(_bodyDefinition->_bodyDef)); // _pPhysicsInterface->CreateBody(&_bodyDefinition->_bodyDef);
	return &output;
}

const void Physics::DestroyBody(PhysicsBody* body)
{
	_pPhysicsInterface->DestroyBody(body->_body);
}

const Vector2 Physics::GetPosition(PhysicsBody* body)
{
	Vector2 output(body->_body->GetPosition().x, body->_body->GetPosition().y);
	return output;
}

//float Physics::GetAngle(PhysicsBody* body)
//{
//	
//}

const float Physics::GetAngleDegress(PhysicsBody* body)
{
	float output = _pPhysicsInterface->GetAngleDegress(body->_body);
	return output;
}

const float Physics::GetAngleRadians(PhysicsBody* body)
{
	float output = _pPhysicsInterface->GetAngleRadians(body->_body);
	return output;
}

const void Physics::SetAngleDegress(PhysicsBody* body, float angle)
{
	_pPhysicsInterface->SetAngleDegress(body->_body, angle);
}

const void Physics::SetAngleRadians(PhysicsBody* body, float angle)
{
	_pPhysicsInterface->SetAngleRadians(body->_body, angle);
}

const void Physics::SetTransform(PhysicsBody* body, Vector2 position, float angleOfRotation)
{
	body->_body->SetTransform(b2Vec2(position.x, position.y), angleOfRotation);
}

const PhysicsTransform Physics::GetTransform(PhysicsBody* objectBody)
{
	PhysicsTransform output;
	output._Transformation = objectBody->_body->GetTransform();
	return output;
}

const b2Joint* Physics::Createjoint(BindObjectsDef* jointDefinition)
{
	return _pWolrd->_world.CreateJoint(&jointDefinition->_jointdefinition);
}

const void Physics::SetLinearVelocity(PhysicsBody* objectBody, Vector2* Velocity)
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

const void Physics::SetAngularVelocity(PhysicsBody* objectBody, float omega)
{
	objectBody->_body->SetAngularVelocity(omega);
}

const void Physics::ApplyForceToPointOnObj(PhysicsBody* objbody, Vector2& force, Vector2& point, bool wake)
{
	b2Vec2 b2ForceIn; b2ForceIn.x = force.x; b2ForceIn.y = force.y;
	b2Vec2 b2PointIn; b2PointIn.x = point.x; b2PointIn.y = force.y;
	objbody->_body->ApplyForce(b2ForceIn, b2PointIn, wake);
}

const void Physics::ApplyForceToObj(PhysicsBody* objBody, Vector2 force, bool wake)
{
	b2Vec2 b2ForceIn; b2ForceIn.x = force.x; b2ForceIn.y = force.y;
	_pPhysicsInterface->ApplyForceToObj(objBody->_body, b2ForceIn, wake);
}

const HitBoxDefnintions Physics::CreateHitBox(Vector2 scale)
{
	b2Vec2 halfScale; halfScale.x = scale.x / 2; halfScale.y = scale.y / 2;

	HitBoxDefnintions output; output._HitBox = _pPhysicsInterface->CreateHitBox(halfScale);
	return output;
}

const void Physics::FixHitboxToBody(PhysicsBody* body, HitBoxDefnintions* hitbox, float density)
{
	_pPhysicsInterface->FixHitboxToBody(body->_body, &hitbox->_HitBox, density);
}

const void Physics::DeleteHitBox(PhysicsBody* body, b2Fixture* fixture)
{
	_pPhysicsInterface->DeleteHitBox(body->_body, fixture);

}

const void Physics::UpdateObject(PhysicsBody* body)
{
}
