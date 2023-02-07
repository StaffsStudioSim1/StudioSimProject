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
	//inline PhysicsWorld() : _world(b2Vec2 in) { }
	//inline PhysicsWorld(_world) : _wolrd(b2Vec2 in) { }

}PhysicsWorld;

struct PhysicsTransform
{
	b2Transform transformation;
};


struct BodyDefinition
{
	BodyDefinition() {};
//	b2BodyDef bodyDef;
	Vector2 Pos;
	float Rotation;
	float density, friction;
	//b2Fixture _fixture;
};

struct HitBoxDefnintions
{
//	b2PolygonShape hitBox;
	PhysicsBodyType bodyType;
	HitboxShape shape;
	Vector2 scale;
};

//struct BindObjectsDef
//{
//	b2JointDef _jointdefinition;
//};
struct Momentum
{
	Vector2 SpeedMPS;
	float WeightAsKg;
};

typedef struct PhysicsBody
{
	PhysicsBody()  { }

//	b2Body* body;
	//PhysicsBodyType _BodyType;
	BodyDefinition bodyDef;
	HitBoxDefnintions hitbox;
	Momentum objMomentum;
	int ObjectID, typeId;

}PhysicsBody;

//struct BodyEdgeCollision
//{
//	b2ContactEdge* edge;
//};


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
