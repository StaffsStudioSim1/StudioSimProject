#pragma once
#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include "Physics/Vectors.h"

enum PhysicsBodyType
{
	Static,
	Kinematic,
	Dynmaic
};

enum HitboxShape
{
	Box,
	Circular
};

typedef struct PhysicsWorld
{
	PhysicsWorld() { world = nullptr; }

	b2World* world;

}PhysicsWorld;

struct PhysicsTransform
{
	b2Transform transformation;
};

struct BodyDefinition
{
	BodyDefinition() {};
	Vector2 Pos;
	float Rotation;
	float density, friction;
};

struct HitBoxDefnintions
{
	PhysicsBodyType bodyType;
	HitboxShape shape;
	Vector2 scale;
};


struct Momentum
{
	Vector2 SpeedMPS;
	float WeightAsKg;
};

typedef struct PhysicsBody
{
	PhysicsBody()  { }

	BodyDefinition bodyDef;
	HitBoxDefnintions hitbox;
	Momentum objMomentum;
	int ObjectID, typeId;

}PhysicsBody;

struct AxisAlignedBoundingBox
{
	Vector2 min;
	Vector2 max;
};
struct PhysicsHitBoxProjection
{
	Vector2 vectorMin;
	Vector2 vectorMax;
};
