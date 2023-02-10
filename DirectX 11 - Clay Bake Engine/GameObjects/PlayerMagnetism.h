#pragma once
#include "Components/Component.h"
#include "../Physics/Vectors.h"
#include "MagnetismObject.h"
#include "PlayerController.h"
#include "../Audio/SoundEffect.h"

class PlayerMagnetism :
    public Component
{
public:
    json Write();

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

    SoundEffect* m_activeSound;
    
private:

    Vector2 _currentHandOffset;
    Vector2 _facingVector = Vector2(1,1);
    MagnetDirection _magnetDirection;
};

