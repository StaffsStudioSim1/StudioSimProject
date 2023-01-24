#pragma once
#include "Component.h"
#include "../Input/PlayerInput.h"

class PlayerController : public Component
{
public:
    void Start() override;
    void Update(float deltaTime) override;
    void FixedUpdate(float timeStep) override;
    void Stop() override;

private:
    PlayerInput* pi;

};

