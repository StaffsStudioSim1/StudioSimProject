#pragma once
#include "Component.h"
#include "../Input/PlayerInput.h"

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

    float _moveSpeed;
    float _topSpeed;
    float _midAirControlDelay;
    float _jumpTimer;

    bool _jumpReset;
    bool _isJumping;
    bool _movementEnabled = true;

    //Functions
    void MovePressed();
    void MoveReleased();
    void JumpPressed();
    void JumpReleased();
    void InteractPressed();
    void MagnetPressed();
    void PausePressed();


    //Physics Reliant Vars
    //Vector2 _currentMovementVector;
    //Vector2 _jumpForce;
    //BoxCollider2D _groundDetector (change to raycast);
    //FacingDirection _facing;
    //Rigidbody2D _rb;

};

