#pragma once
#include "Component.h"
#include "../../Physics/Vectors.h"
#include "AABB.h"
class Rigidbody :
    public Component
{
public:
    Rigidbody();

    void FixedUpdate(float timeStep);

    void AddForce(Vector2 force);
    void Collide(GameObject* victim);
    Vector2 GetVelocity();
private:
    Vector2 _velocity;
    Vector2 _acceleration;
    Vector2 _netForce;

    float _mass;
};

