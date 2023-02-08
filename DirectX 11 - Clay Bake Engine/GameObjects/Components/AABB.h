#pragma once
#include "Component.h"
#include "../../Physics/Vectors.h"
#include <DirectXCollision.h>
class AABB :
    public Component
{
public:
    AABB(float width, float height);

    bool Overlaps(AABB* collider, float deltaTime);
    Vector2 GetSize();
private:
    float _width;
    float _height;
};

