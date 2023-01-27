#pragma once
#include "Component.h"
#include "../Physics/Vectors.h"
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
    
    Vector2* m_boxSize;    
    Vector2* m_handOffest;
private:

    Vector2* m_currentHandOffset;
    Vector2* m_factingVector = new Vector2(1,1);
    
};
