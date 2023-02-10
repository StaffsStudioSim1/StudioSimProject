#pragma once
#include "MagnetismObject.h"
#include "Components/Rigidbody.h"
#include "GameObject.h"

class MagnetBox :
    public MagnetismObject
{
public:
    void Magnetise(Vector2 pushingPlayerPos, MagnetDirection forceDirection, PlayerController* ControlingPlayer);
    void Start();
    void Stop();

    json Write();
private:
    Rigidbody* _rigidbody;
    float _pushForce = 2.5f;
};


