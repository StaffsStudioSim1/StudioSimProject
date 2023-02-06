#pragma once
#include "Component.h"
#include "../Physics/Vectors.h"
#include "GameObject.h"
#include "Physics.h"

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
    void SetPlatfromDirection(PlatformDirection newDirection);
    Vector2* m_limit1;
    Vector2* m_limit2;

    void Update(float timeStep);

    void Start();
    void Stop();

private:
    PlatformDirection m_movingDirection;
    PlatformDirection m_lastDirection;
    Vector2* m_oragin;

    Vector2 MoveTowards(Vector2 currentPos, Vector2 Target, float deltaTime);
    
    
    float Lerp(float a,float b,float t);
    float Clamp(float v, float lo, float hi);
    
    Physics* m_Physics;

};

