#pragma once
#include "Component.h"
//#include "../Input/PlayerInput.h"

enum PlayerDirection
{
    Left,
    Right
}
;
class PlayerMagnetism :
    public Component
{
public:

    void Start();
    void Stop();
    void FixedUpdate(float timeStep);

    //void ChangeDirection(PlayerDirection direction);
    
    void ChangeDirection(PlayerDirection direction);
    
    void MagnetOn();
    void MagnetOff();

    bool m_active;
    //Vector2 box size
    
    //Vector2 HandOffset
    // 
private:

    //Vector2 currentHandOffset
    //Vector2 facing vector
};

