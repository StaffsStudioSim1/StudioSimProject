#pragma once
#include "Component.h"
#include"../Physics/Vectors.h"
#include "PlayerController.h"

enum MagnetDirection
{
    Towards,
    Away
};

class MagnetismObject :
    public Component
{
public:
    virtual void isBeingMagnetised(Vector2 pushingPlayerPos, MagnetDirection forceDirection, PlayerController* ControlingPlayer) = 0;
};

