#pragma once
#include "Components/Component.h"
#include "../Physics/Vectors.h"
#include "GameObject.h"
#include "Components/Rigidbody.h"

enum PlatformDirection
{
    PlatfromLeft,
    None,
    PlatfromRight
};

class MovingPlatform :
    public Component
{
public:
    
    MovingPlatform(float limit1X, float limit1Y, float limit2X, float limit2Y);
    void SetPlatfromDirection(PlatformDirection newDirection);
    Vector2 m_limit1;
    Vector2 m_limit2;

    void Update(float deltaTime);

    void Start();
    void Stop();

private:
    PlatformDirection m_movingDirection;
    PlatformDirection m_lastDirection;
    Vector2 m_origin;

    Vector2 MoveTowards(Vector2 currentPos, Vector2 Target, float deltaTime);
    
    
    float Lerp(float a,float b,float t);
    float Clamp(float v, float lo, float hi);
    
    

};

