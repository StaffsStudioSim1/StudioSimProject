#include "Physics.h"
#include "ObjectHandler.h"

Physics::Physics(PhysicsBody* body, Vector2 inputGravity) // Initilaise transform variables
{
	//_pTransform = transform;

	_objectPhysicsBody = body;
	_pGravityAsForce = inputGravity;

	SetTransform(body->bodyDef.Pos, body->bodyDef.Rotation);

}

Physics::~Physics()
{
	if (_objectPhysicsBody)
		delete _objectPhysicsBody;
}

json Physics::Write()
{
	json me;
	me[JSON_COMPONENT_CLASS] = "Physics";
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(GetBodyType());
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(GetDensity());
	me[JSON_COMPONENT_CONSTRUCTORS].push_back(GetFriction());
	return me;
}

void Physics::Stop()
{
	if (_objectPhysicsBody)
	{
		delete _objectPhysicsBody;
		_objectPhysicsBody = nullptr;
	}
}

void Physics::Update()
{
	_gameObject->GetTransform()->SetPosition(GetPosition());
}

PhysicsBody* Physics::GetPhysicsBody()
{
	return _objectPhysicsBody;
}

Vector2 Physics::GetPosition()
{
	return _objectPhysicsBody->bodyDef.Pos;
}

float Physics::GetAngleDegress()
{
	return RAD2DEG(_objectPhysicsBody->bodyDef.Rotation);
}

float Physics::GetAngleRadians()
{
	return _objectPhysicsBody->bodyDef.Rotation;
}

void Physics::SetAngleDegress(float angle)
{
	_objectPhysicsBody->bodyDef.Rotation = DEG2RAD(angle);
}

void Physics::SetAngleRadians(float angle)
{
	_objectPhysicsBody->bodyDef.Rotation = angle;
}

void Physics::SetTransform(Vector2 position, float angleOfRotation)
{
	_objectPhysicsBody->bodyDef.Pos = position; 
	_objectPhysicsBody->bodyDef.Rotation = angleOfRotation;
}

Vector2 Physics::GetTransform()
{
	return _objectPhysicsBody->bodyDef.Pos;
	
}

void Physics::SetLinearVelocity(Vector2 velocity)
{
	_objectPhysicsBody->objMomentum.SpeedMPS = velocity;
}

Vector2 Physics::GetLinearVelocity()
{
	Vector2* swapper = &_objectPhysicsBody->objMomentum.SpeedMPS;
	return *swapper;
}

void Physics::ApplyForceToObj(Vector2 force)
{
	_objectPhysicsBody->objMomentum.SpeedMPS += force;
}


void Physics::ApplyImpulseForceToObj(Vector2 force)
{
	_objectPhysicsBody->objMomentum.SpeedMPS += force * 10.0f;

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
		if (box1.Intersects(box2) && object->GetID() != 0)
		{
			if (ObjectHandler::GetInstance().GetGameObject(object->GetID())->GetComponent<Physics>())
			{
				currentnumberOfObjects += 1;
			}
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
		if (ObjectHandler::GetInstance().GetGameObject(object->GetID())->GetComponent<Physics>() != nullptr)
		{
		DirectX::XMFLOAT2 boxScale = object->GetTransform()->GetScale();
		Vector2 boxPos = object->GetTransform()->GetPosition();
		box2.Extents = { boxScale.x, boxScale.y, 0.0f };
		box2.Center = { boxPos.x, boxPos.y, 0.0f };
			if (box1.Intersects(box2) && object->GetID() != 0) // change to get object tag and param to null
			{
				outputBodyArray.push_back(object->GetID());
				currentnumberOfObjects += 1;

			}
		}
	}

	return outputBodyArray;
}


bool Physics::IsObjectCollidingwith(PhysicsBody input)
{
	bool contact = false;
	
	std::vector<int> collisions = GetObjectsCollisionsByID(_objectPhysicsBody->bodyDef.Pos, _objectPhysicsBody->hitbox);

	return contact;
}

std::vector<int> Physics::GetObjectsCollisionsByID(Vector2 position, HitBoxDefnintions hitBoxDef)
{
	return GetObjectsInAreaByID(position, hitBoxDef.scale);
}

int Physics::GetNumberOfCollisonsWithBody(Vector2 position, HitBoxDefnintions hitboxdef)
{
	return GetNumberOfObjectsInArea(position, hitboxdef.scale);
}


void Physics::PhysicsStaticCollision(int ObjectID)
{
	GameObject* secondobject = ObjectHandler::GetInstance().GetGameObject(ObjectID);

	Vector2 _momentum = secondobject->GetComponent<Physics>()->GetPhysicsBody()->objMomentum.SpeedMPS;
	secondobject->GetComponent<Physics>()->ApplyForceToObj(-_momentum);

}

void Physics::PhysicsDynamicCollision(int objectID)
{
	GameObject* secondobject = ObjectHandler::GetInstance().GetGameObject(objectID);
	
	Vector2 _momentum = secondobject->GetComponent<Physics>()->GetPhysicsBody()->objMomentum.SpeedMPS;
	secondobject->GetComponent<Physics>()->ApplyForceToObj(-_momentum);
}

void Physics::ApplyGravityForceForUpdate()
{
	Momentum workingMomentum_Kg_Ms = _objectPhysicsBody->objMomentum;
	Vector2 _momentum = workingMomentum_Kg_Ms.SpeedMPS * workingMomentum_Kg_Ms.WeightAsKg;
	Vector2 objectStartingpos = _objectPhysicsBody->bodyDef.Pos;

	_momentum += _pGravityAsForce;
	_objectPhysicsBody->bodyDef.Pos += _momentum / workingMomentum_Kg_Ms.WeightAsKg;

}
//
//void Physics::updateBodyForces()
//{
//	if (_objectPhysicsBody->hitbox.bodyType == Dynmaic)
//	{ApplyGravityForceForUpdate();}
//
//	std::vector<int> listOfIDs = GetObjectsCollisionsByID(_objectPhysicsBody->bodyDef.Pos, _objectPhysicsBody->hitbox);
//	
//	for (int b6 : listOfIDs)
//	{
//		switch (ObjectHandler::GetInstance().GetGameObject(b6)->GetComponent<Physics>()->GetPhysicsBody()->hitbox.bodyType)
//		{
//		case Static:
//			PhysicsStaticCollision(b6);
//			break;
//		case Kinematic:
//			PhysicsStaticCollision(b6); // "temp"
//			break;
//		case Dynmaic:
//			PhysicsDynamicCollision(b6);
//			break;
//
//		default:
//			break;
//		}
//	}
//	Update();
//}

void Physics::FixedUpdate(float timeStep)
{
	if (_objectPhysicsBody->hitbox.bodyType == Dynmaic)
	{
		Vector2 objectForce = GetSteppedGravityForce();
		_objectPhysicsBody->bodyDef.Pos += objectForce;
	}

	std::vector<int> listOfIDs = GetObjectsCollisionsByID(_objectPhysicsBody->bodyDef.Pos, _objectPhysicsBody->hitbox);

	
	for(int b6 : listOfIDs)
	{
		if (b6 != 0 && _objectPhysicsBody->ObjectID != 0)
		{
			switch (_objectPhysicsBody->hitbox.bodyType)
			{
			case Static:
				PhysicsStaticCollision(b6);
				OutputDebugStringA("static collision \n");
				break;
			case Kinematic:
				PhysicsStaticCollision(b6); // "temp"
				OutputDebugStringA("kinematic collision \n");
				break;
			case Dynmaic:
				PhysicsDynamicCollision(b6);
				OutputDebugStringA("dynamic collision \n");
				break;

			default:
				break;
			}
		}
	}
	Update();
}

Vector2 Physics::GetSteppedGravityForce()
{
	Momentum workingMomentum_Kg_Ms = _objectPhysicsBody->objMomentum;
	Vector2 _steppedmomentum = workingMomentum_Kg_Ms.SpeedMPS * workingMomentum_Kg_Ms.WeightAsKg;
	Vector2 objectStartingpos = _objectPhysicsBody->bodyDef.Pos;

	_steppedmomentum += _pGravityAsForce;
	_steppedmomentum /= workingMomentum_Kg_Ms.WeightAsKg;
	//_steppedmomentum /= 5.0f;
	
	return _steppedmomentum;
}