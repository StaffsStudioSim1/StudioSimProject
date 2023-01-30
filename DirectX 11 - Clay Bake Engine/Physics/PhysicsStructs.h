#pragma once
#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include "Vectors.h"

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

struct PhysicsWorld
{
	b2World _world;
};

struct PhysicsTransform
{
	b2Transform _Transformation;
};

struct PhysicsBody
{
	b2Body* _body;
	
};

struct BindObjectsDef
{
	b2JointDef _jointdefinition;
};


struct BodyDefinition
{
	b2BodyDef _bodyDef;
};

struct HitBoxDefnintions
{
	b2PolygonShape _HitBox;
	PhysicsBodyType _BodyType;
	HitboxShape _Shape;
	float _scaleX, _ScaleY;
};