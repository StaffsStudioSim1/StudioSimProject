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

    float _moveSpeed;
    float _topSpeed = 10.0f;
    float _midAirControlDelay;
    float _jumpTimer;

    bool _jumpReset;
    bool _isJumping;
    bool _movementEnabled = true;

    Vector2 _currentMovement;
    Vector2 _jumpForce = {0.0f, 10.0f};

    //Functions
    void MovePressed(Vector2 direction);
    void MoveReleased();
    void JumpPressed();
    void JumpReleased();
    void InteractPressed();
    void MagnetPressed();
    void MagnetReleased();
    void PausePressed();


    //Physics Reliant Vars
    //BoxCollider2D _groundDetector (change to raycast);
    //FacingDirection _facing;
    //Rigidbody2D _rb;

};

