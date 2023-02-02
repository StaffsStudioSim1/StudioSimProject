#pragma once
#include "Component.h"
#include "../Input/PlayerInput.h"
#include "../Physics/Vectors.h"
#include "Physics.h"
#include "GameObject.h"

class PlayerController : public Component
{
public:

    //Variables
    bool isFlipped;

    //Functions
    void Start() override;
    void Update(float deltaTime) override;
    void FixedUpdate(float timeStep) override;
    void Stop() override;

private:

    //Variables
    PlayerInput* _playerInput;
    Physics* _physicsBody;

    float _moveSpeed = 10000.0f;
    float _topSpeed = 10000.0f;
    float _midAirControlDelay;
    float _jumpTimer;

    bool _jumpReset = true;
    bool _isJumping = false;
    bool _movementEnabled = true;

    Vector2 _currentMovement;
    Vector2 _jumpForce = {0.0f, 10000.0f};

    //Functions
    void JumpPressed();
    void JumpReleased();
    void InteractPressed();
    void MagnetPressed();
    void MagnetReleased();
    void PausePressed();

    //Physics Reliant Vars
    //BoxCollider2D _groundDetector (change to raycast);
    //FacingDirection _facing;
};

