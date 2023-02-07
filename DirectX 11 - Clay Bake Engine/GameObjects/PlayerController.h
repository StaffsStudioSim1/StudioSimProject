#pragma once
#include "Components/Component.h"
#include "Components/Rigidbody.h"
#include "../Input/PlayerInput.h"
#include "../Physics/Vectors.h"
#include "../Audio/SoundEffect.h"
#include "GameObject.h"
#include "GameManager.h"

enum FacingDirection
{
    Left,
    Right
};

class PlayerController : public Component
{
public:

    //Functions
    PlayerController(int id);
    ~PlayerController();

    json Write() override;
    void Start() override;
    void Update(float deltaTime) override;
    void FixedUpdate(float timeStep) override;
    void Stop() override;

private:

    //Variables
    PlayerInput* _playerInput;
    Rigidbody* _rigidbody;
    SoundEffect* _jumpSoundEffect;
    SoundEffect* _moveSoundEffect;

    int _playerID;
    float _moveSpeed = 100.0f;
    float _topSpeed = 10000.0f;
    float _midAirControlDelay;
    float _jumpTimer;

    bool _jumpReset = true;
    bool _isJumping = false;
    bool _movementEnabled = true;
    bool _isWalking = false;

    Vector2 _currentMovement;
    Vector2 _jumpForce = {0.0f, 10000.0f};
    FacingDirection _facingDirection;

    //Functions
    void JumpPressed();
    void JumpReleased();
    void InteractPressed();
    void MagnetPressed();
    void MagnetReleased();
    void PausePressed();

    //Physics Reliant Vars
    //BoxCollider2D _groundDetector (change to raycast);
};
