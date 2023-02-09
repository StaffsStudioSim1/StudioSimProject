#pragma once
#include "Components/Component.h"
#include "../Physics/Vectors.h"
#include "MagnetismObject.h"
#include "PlayerController.h"


class PlayerMagnetism :
    public Component
{
public:

    void Start();
    void Stop();
    void FixedUpdate(float timeStep);

    //void ChangeDirection(PlayerDirection direction);
    
    void ChangeDirection(FacingDirection direction);
    
    void MagnetOn();
    void MagnetOff();

    bool m_magnetActive;
    
    Vector2 m_boxSize;    
    Vector2 m_handOffest;

    void SetMagnetPushPull(int playerID);
    
private:

    Vector2 m_currentHandOffset;
    Vector2 m_factingVector = Vector2(1,1);
    MagnetDirection m_magnetDirection;
};

