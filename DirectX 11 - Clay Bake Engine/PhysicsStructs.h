#pragma once
#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include "Physics/Vectors.h"

enum PhysicsBodyType
{
	_Static,
	_kinematic,
	_Dynmaic
};

enum HitboxShape
{
	_box, 
	_circular
};

typedef struct PhysicsWorld
{

	b2World _world;
//	inline PhysicsWorld() : _wolrd(b2Vec2 in) { }
	//inline PhysicsWorld(_world) : _wolrd(b2Vec2 in) { }

}PhysicsWorld;

struct PhysicsTransform
{
	b2Transform _Transformation;
};


struct BodyDefinition
{
	b2BodyDef _bodyDef;
	Vector2 StartPos;
	float StartingRoatation;
	float density, friction;
	b2Fixture _fixture;
};

struct HitBoxDefnintions
{
	b2PolygonShape _HitBox;
	PhysicsBodyType _BodyType;
	HitboxShape _Shape;
	float _scaleX, _ScaleY;
};

struct BindObjectsDef
{
	b2JointDef _jointdefinition;
};

typedef struct PhysicsBody
{
	b2Body* _body;
	//PhysicsBodyType _BodyType;
	BodyDefinition _bodyDef;
	HitBoxDefnintions hitboxdef;
};

