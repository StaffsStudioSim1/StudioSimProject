#include "Physics.h"
#include "ObjectHandler.h"

Physics::Physics(PhysicsBody* body, PhysicsWorld* world) // Initilaise transform variables
{
	//_pTransform = transform;
	_pWorld = world;
	//_pTransform = _gameObject->GetTransform();
	_pPhysicsInterface = new PhysicsInterface(_pWorld->_world);

	_objectPhysicsBody = body;


	body->_bodyDef = SetCorrectBodyDef(*body, body->hitboxdef._BodyType);

	body->_body = world->_world->CreateBody(&body->_bodyDef._bodyDef);

	SetTransform(body->_bodyDef.StartPos, body->_bodyDef.StartingRoatation);

	body->hitboxdef = CreateHitBox(Vector2(body->hitboxdef._scaleX, body->hitboxdef._ScaleY));
	b2Fixture* _fixture = body->_body->CreateFixture(&body->hitboxdef._HitBox, body->_bodyDef.density);
}

Physics::~Physics()
{
	// TODO Destroy Body
}

void Physics::Update(float deltaTime) // Update physics
{
	//	_pPhysicsInterface->Update(deltaTime);
	_gameObject->GetTransform()->SetPosition(GetPosition());
	
}

PhysicsInterface Physics::GetPhysicInterface()
{
	return *_pPhysicsInterface;
}

PhysicsWorld* Physics::Getworld()
{
	return _pWorld;
}

PhysicsBody* Physics::CreateBody(PhysicsBody* _body)
{
	_body->_body = _pPhysicsInterface->CreateBody(&_body->_bodyDef._bodyDef);
	return _body;
}

Vector2 Physics::GetPosition()
{
	Vector2 output(_objectPhysicsBody->_body->GetPosition().x, _objectPhysicsBody->_body->GetPosition().y);
	return output;
}

//float Physics::GetAngle()
//{
//	
//}

float Physics::GetAngleDegress()
{
	float output = _pPhysicsInterface->GetAngleDegress(_objectPhysicsBody->_body);
	return output;
}

float Physics::GetAngleRadians()
{
	float output = _pPhysicsInterface->GetAngleRadians(_objectPhysicsBody->_body);
	return output;
}

void Physics::SetAngleDegress(float angle)
{
	_pPhysicsInterface->SetAngleDegress(_objectPhysicsBody->_body, angle);
}

void Physics::SetAngleRadians(float angle)
{
	_pPhysicsInterface->SetAngleRadians(_objectPhysicsBody->_body, angle);
}

void Physics::SetTransform(Vector2 position, float angleOfRotation)
{
	_objectPhysicsBody->_body->SetTransform(b2Vec2(position.x, position.y), angleOfRotation);
}

PhysicsTransform Physics::GetTransform()
{
	PhysicsTransform output;
	output._Transformation = _objectPhysicsBody->_body->GetTransform();
	return output;
}

b2Joint* Physics::Createjoint(BindObjectsDef* jointDefinition)
{
	return _pWorld->_world->CreateJoint(&jointDefinition->_jointdefinition);
}

void Physics::SetLinearVelocity(Vector2* Velocity)
{
	b2Vec2 input;
	input.x = Velocity->x; input.y = Velocity->y;
	_pPhysicsInterface->SetLinearVelocity(_objectPhysicsBody->_body, &input);
}

Vector2* Physics::GetLinearVelocity()
{
	Vector2 output;
	b2Vec2* swapper = _pPhysicsInterface->GetLinearVelocity(_objectPhysicsBody->_body);
	output.x = swapper->x; output.y = swapper->y;
	return &output;
}

void Physics::SetAngularVelocity(float omega)
{
	_objectPhysicsBody->_body->SetAngularVelocity(omega);
}

void Physics::ApplyForceToPointOnObj(Vector2& force, Vector2& point, bool wake)
{
	b2Vec2 b2ForceIn; b2ForceIn.x = force.x; b2ForceIn.y = force.y;
	b2Vec2 b2PointIn; b2PointIn.x = point.x; b2PointIn.y = force.y;
	_objectPhysicsBody->_body->ApplyForce(b2ForceIn, b2PointIn, wake);
}

void Physics::ApplyForceToObj(Vector2 force, bool wake)
{
	b2Vec2 b2ForceIn; b2ForceIn.x = force.x; b2ForceIn.y = force.y;
	_pPhysicsInterface->ApplyForceToObj(_objectPhysicsBody->_body, b2ForceIn, wake);
}

HitBoxDefnintions Physics::CreateHitBox(Vector2 scale)
{
	b2Vec2 halfScale; halfScale.x = scale.x * 13; halfScale.y = scale.y * 13;

	HitBoxDefnintions output; output._HitBox = _pPhysicsInterface->CreateHitBox(halfScale);
	return output;
}

void Physics::FixHitboxToBody(HitBoxDefnintions* hitbox, float density)
{
	_pPhysicsInterface->FixHitboxToBody(_objectPhysicsBody->_body, &hitbox->_HitBox, density);
}

void Physics::DeleteHitBox(b2Fixture* fixture)
{
	_pPhysicsInterface->DeleteHitBox(_objectPhysicsBody->_body, fixture);

}

BodyDefinition Physics::SetCorrectBodyDef(PhysicsBody input, PhysicsBodyType type)
{
	BodyDefinition output = input._bodyDef;
	switch (type)
	{
	case _Static:
		output._bodyDef.type = b2_staticBody;
		break;
	case _kinematic:
		output._bodyDef.type = b2_kinematicBody;
		break;
	case _Dynmaic:
		output._bodyDef.type = b2_dynamicBody;
		break;
	default:
		output._bodyDef.type = b2_dynamicBody;
		break;
	}
	return output;
}


b2World* Physics::CreatePhysicsWorld(float gravity)
{

	b2World boxWorld(b2Vec2(0.0f, gravity));

	return &boxWorld;
}
