#pragma once
#include "Components/Component.h"
#include"../Physics/Vectors.h"
#include "PlayerController.h"

enum MagnetDirection
{
    PullTowards,
    PushAway
};

class MagnetismObject :
    public Component
{
public:
    virtual void Magnetise(Vector2 pushingPlayerPos, MagnetDirection forceDirection, PlayerController* ControlingPlayer) = 0;
};

