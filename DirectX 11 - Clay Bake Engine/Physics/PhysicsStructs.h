#pragma once
#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include "Vectors.h"

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
};


struct BodyEdgeCollision
{
	b2ContactEdge* edge;
};
