#pragma once
#include "MagnetismObject.h"
#include "Physics.h"
#include "GameObject.h"

class MagnetBox :
    public MagnetismObject
{
public:
    void isBeingMagnetised(Vector2 pushingPlayerPos, MagnetDirection forceDirection, PlayerController* ControlingPlayer);
    void Start();
    void Stop();

    Physics* m_Physics;

    float m_PushForce = 100.0f;
};


