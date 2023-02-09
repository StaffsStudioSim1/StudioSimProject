#pragma once
#include "MagnetismObject.h"
#include "Components/Rigidbody.h"
#include "GameObject.h"

class MagnetBox :
    public MagnetismObject
{
public:
    void isBeingMagnetised(Vector2 pushingPlayerPos, MagnetDirection forceDirection, PlayerController* ControlingPlayer);
    void Start();
    void Stop();

    Rigidbody* m_rigidbody;

    float m_PushForce = 1.0f;
};


