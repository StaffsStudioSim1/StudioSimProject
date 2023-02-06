#include "Physics.h"
#include "ObjectHandler.h"

Physics::Physics(PhysicsBody* body, PhysicsWorld* world) // Initilaise transform variables
{
	//_pTransform = transform;
	_pWorld = world;
	//_pTransform = _gameObject->GetTransform();
	_pPhysicsInterface = new PhysicsInterface(_pWorld->world);

	_objectPhysicsBody = body;


	body->bodyDef = SetCorrectBodyDef(*body, body->hitboxdef.bodyType);

	body->body = world->world->CreateBody(&body->bodyDef.bodyDef);

	SetTransform(body->bodyDef.startPos, body->bodyDef.startingRoatation);

	body->hitboxdef = CreateHitBox(Vector2(body->hitboxdef.scaleX, body->hitboxdef.scaleY));
	b2Fixture* _fixture = body->body->CreateFixture(&body->hitboxdef.hitBox, body->bodyDef.density);
}

Physics::~Physics()
{
	if (_objectPhysicsBody)
		delete _objectPhysicsBody;
}

json Physics::Write()
{
	json me;
	bool isDynamic = _objectPhysicsBody->hitboxdef.bodyType == Dynmaic ? true : false;
	me[JSON_COMPONENT_CLASS] = "Physics";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(isDynamic);
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(GetDensity());
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(GetFriction());
	return me;
}

void Physics::Update(float deltaTime)
{
	_gameObject->GetTransform()->SetPosition(GetPosition());
}

PhysicsInterface Physics::GetPhysicInterface()
{
	return *_pPhysicsInterface;
}

PhysicsWorld* Physics::GetWorld()
{
	return _pWorld;
}

PhysicsBody* Physics::CreateBody(PhysicsBody* _body)
{
	_body->body = _pPhysicsInterface->CreateBody(&_body->bodyDef.bodyDef);
	return _body;
}

PhysicsBody* Physics::GetPhysicsBody()
{
	return _objectPhysicsBody;
}

Vector2 Physics::GetPosition()
{
	Vector2 output(_objectPhysicsBody->body->GetPosition().x, _objectPhysicsBody->body->GetPosition().y);
	return output;
}

float Physics::GetAngleDegress()
{
	float output = _pPhysicsInterface->GetAngleDegress(_objectPhysicsBody->body);
	return output;
}

float Physics::GetAngleRadians()
{
	float output = _pPhysicsInterface->GetAngleRadians(_objectPhysicsBody->body);
	return output;
}

void Physics::SetAngleDegress(float angle)
{
	_pPhysicsInterface->SetAngleDegress(_objectPhysicsBody->body, angle);
}

void Physics::SetAngleRadians(float angle)
{
	_pPhysicsInterface->SetAngleRadians(_objectPhysicsBody->body, angle);
}

void Physics::SetTransform(Vector2 position, float angleOfRotation)
{
	_objectPhysicsBody->body->SetTransform(b2Vec2(position.x, position.y), angleOfRotation);
}

PhysicsTransform Physics::GetTransform()
{
	PhysicsTransform output;
	output.transformation = _objectPhysicsBody->body->GetTransform();
	return output;
}

b2Joint* Physics::CreateJoint(BindObjectsDef* jointDefinition)
{
	return _pWorld->world->CreateJoint(&jointDefinition->_jointdefinition);
}

void Physics::SetLinearVelocity(Vector2 velocity)
{
	b2Vec2 input;
	input.x = velocity.x;
	input.y = velocity.y;
	_pPhysicsInterface->SetLinearVelocity(_objectPhysicsBody->body, &input);
}

Vector2 Physics::GetLinearVelocity()
{
	b2Vec2* swapper = _pPhysicsInterface->GetLinearVelocity(_objectPhysicsBody->body);
	return Vector2(swapper->x, swapper->y);
}

void Physics::SetAngularVelocity(float omega)
{
	_objectPhysicsBody->body->SetAngularVelocity(omega);
}

void Physics::ApplyForceToPointOnObj(Vector2& force, Vector2& point, bool wake)
{
	b2Vec2 b2ForceIn; b2ForceIn.x = force.x; b2ForceIn.y = force.y;
	b2Vec2 b2PointIn; b2PointIn.x = point.x; b2PointIn.y = force.y;
	_objectPhysicsBody->body->ApplyForce(b2ForceIn, b2PointIn, wake);
}

void Physics::ApplyForceToObj(Vector2 force, bool wake)
{
	b2Vec2 b2ForceIn; b2ForceIn.x = force.x; b2ForceIn.y = force.y;
	_pPhysicsInterface->ApplyForceToObj(_objectPhysicsBody->body, b2ForceIn, wake);
}

void Physics::ApplyImpulseForceToPointOnObj(Vector2& force, Vector2& point, bool wake)
{
	b2Vec2 b2forceIn; b2forceIn.x = force.x; b2forceIn.y = force.y;
	b2Vec2 b2PointIn; b2PointIn.x = point.x; b2PointIn.y = force.y;
	_pPhysicsInterface->ApplyImpulseForceToPointOnObj(_objectPhysicsBody->body, b2forceIn, b2PointIn, wake);

}

void Physics::ApplyImpulseForceToObj(Vector2 force, bool wake)
{
	b2Vec2 b2forceIn; b2forceIn.x = force.x; b2forceIn.y = force.y;
	_pPhysicsInterface->ApplyImpulseForceToObj(_objectPhysicsBody->body, b2forceIn, wake);
}

HitBoxDefnintions Physics::CreateHitBox(Vector2 scale)
{
	b2Vec2 halfScale; halfScale.x = scale.x * 6.5f; halfScale.y = scale.y * 6.5f;

	HitBoxDefnintions output; output.hitBox = _pPhysicsInterface->CreateHitBox(halfScale);
	return output;
}

void Physics::FixHitboxToBody(HitBoxDefnintions* hitbox, float density)
{
	_pPhysicsInterface->FixHitboxToBody(_objectPhysicsBody->body, &hitbox->hitBox, density);
}

void Physics::DeleteHitBox(b2Fixture* fixture)
{
	_pPhysicsInterface->DeleteHitBox(_objectPhysicsBody->body, fixture);

}


int Physics::GetNumberOfObjectsInArea(Vector2 position, Vector2 areaScale)
{
	int currentnumberOfObjects = 0;
	DirectX::BoundingBox box1;
	box1.Extents = { areaScale.x, areaScale.y, 0.0f }; // areaPojection
	box1.Center = { position.x,position.y, 0.0f };//projected point


	DirectX::BoundingBox box2;
	for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
	{
		DirectX::XMFLOAT2 boxScale = object->GetTransform()->GetScale();
		Vector2 boxPos = object->GetTransform()->GetPosition();
		box2.Extents = { boxScale.x, boxScale.y, 0.0f };
		box2.Center = { boxPos.x, boxPos.y, 0.0f };
		if (box1.Intersects(box2))
		{
			currentnumberOfObjects += 1;
		}
	}

	return currentnumberOfObjects;
}

std::vector<int> Physics::GetObjectsInAreaByID(Vector2 position, Vector2 areaScale)
{
	std::vector<int> outputBodyArray;
	int currentnumberOfObjects = 0;
	DirectX::BoundingBox box1;
	box1.Extents = { areaScale.x, areaScale.y, 0.0f }; // areaPojection
	box1.Center = { position.x,position.y, 0.0f };//projected point


	DirectX::BoundingBox box2;
	for (GameObject* object : ObjectHandler::GetInstance().GetAllObjects())
	{
		DirectX::XMFLOAT2 boxScale = object->GetTransform()->GetScale();
		Vector2 boxPos = object->GetTransform()->GetPosition();
		box2.Extents = { boxScale.x, boxScale.y, 0.0f };
		box2.Center = { boxPos.x, boxPos.y, 0.0f };
		if (box1.Intersects(box2))
		{
			outputBodyArray.push_back(object->GetID());
			currentnumberOfObjects += 1;
		}
	}

	return outputBodyArray;
}


BodyDefinition Physics::SetCorrectBodyDef(PhysicsBody input, PhysicsBodyType type)
{
	BodyDefinition output = input.bodyDef;
	switch (type)
	{
	case Static:
		output.bodyDef.type = b2_staticBody;
		break;
	case Kinematic:
		output.bodyDef.type = b2_kinematicBody;
		break;
	case Dynmaic:
		output.bodyDef.type = b2_dynamicBody;
		break;
	default:
		output.bodyDef.type = b2_dynamicBody;
		break;
	}
	return output;
}


b2World* Physics::CreatePhysicsWorld(float gravity)
{

	b2World boxWorld(b2Vec2(0.0f, gravity));

	return &boxWorld;
}

PhysicsBody Physics::GetCollisionsWithBody()
{
	BodyEdgeCollision collisioncheck;
	b2Fixture* collidingWith = nullptr;
	for (collisioncheck.edge = _objectPhysicsBody->body->GetContactList(); &collisioncheck.edge; collisioncheck.edge = collisioncheck.edge->next)
	{
		collidingWith = collisioncheck.edge->contact->GetFixtureB();
	}

	PhysicsBody output;
	output.body = collidingWith->GetBody();

	return output;

}

bool Physics::IsObjectCollidingwith(PhysicsBody input)
{
	b2Body* test;
	bool contact = false;
	
	for (b2ContactEdge* edge = _objectPhysicsBody->body->GetContactList(); edge != nullptr; edge = edge->next)
	{
		if (edge->other == input.body && edge->contact->IsTouching())
		{
				contact = true;
				break;
		}
	}

	return contact;
}