#pragma once
#include "MagnetismObject.h"
#include "../Physics/PhysicsInterface.h"
class MagnetBox :
    public MagnetismObject
{
public:
    void isBeingMagnetised(Vector2 pushingPlayerPos, MagnetDirection forceDirection, PlayerController* ControlingPlayer);
    void Start();
    void Stop();

    PhysicsInterface* m_attachedPhysics;
};


