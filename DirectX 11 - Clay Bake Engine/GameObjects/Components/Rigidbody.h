#pragma once
#include "Component.h"
#include "../../Physics/Vectors.h"
#include "AABB.h"
class Rigidbody :
    public Component
{
public:
    Rigidbody();

    json Write();

    void FixedUpdate(float timeStep);

    void AddForce(Vector2 force);
    void SetInput(Vector2 input);
    void Collide(GameObject* victim);
    Vector2 GetVelocity();
private:
    Vector2 _velocity;
    Vector2 _acceleration;
    Vector2 _netForce;
    Vector2 _input;

    float _mass;
    float _frictionCoefficient = 1.0f;
};

