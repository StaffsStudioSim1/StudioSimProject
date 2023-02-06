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
	b2BodyDef bodyDef;
	Vector2 startPos;
	float startingRoatation;
	float density, friction;
	//b2Fixture _fixture;
};

struct HitBoxDefnintions
{
	b2PolygonShape hitBox;
	PhysicsBodyType bodyType;
	HitboxShape shape;
	float scaleX, scaleY;
};

struct BindObjectsDef
{
	b2JointDef _jointdefinition;
};

typedef struct PhysicsBody
{
	PhysicsBody()  { }

	b2Body* body;
	//PhysicsBodyType _BodyType;
	BodyDefinition bodyDef;
	HitBoxDefnintions hitboxdef;
}PhysicsBody;


struct BodyEdgeCollision
{
	b2ContactEdge* edge;
};



struct Momentum
{
	float SpeedX;
	float SpeedY;

	float WeightAsKg;

	float TotalSpeed = SpeedX + SpeedY;
	float KilogramMeterPerSecond = TotalSpeed * WeightAsKg;
};

struct DirectinalGravity
{
	float SpeedX;
	float SpeedY;
};


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
