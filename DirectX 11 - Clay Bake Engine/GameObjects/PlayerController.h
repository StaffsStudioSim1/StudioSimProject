#pragma once
#include "Components/Component.h"
#include "Components/Rigidbody.h"
#include "../Input/PlayerInput.h"
#include "../Physics/Vectors.h"
#include "../Audio/SoundEffect.h"
#include "GameObject.h"
#include "GameManager.h"
//#include "PlayerMagnetism.h"

class PlayerMagnetism;

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
    PlayerMagnetism* _magnet;

    int _playerID;
    float _moveSpeed = 100.0f;
    float _topSpeed = 10000.0f;
    float _jumpTimer = 2.0f;
    float _activeJumpTimer = 0.0f;

    bool _jumpReset = true;
    bool _movementEnabled = true;
    bool _isWalking = false;

    Vector2 _currentMovement;
    Vector2 _jumpForce = {0.0f, 7000.0f};
    FacingDirection _facingDirection;

    //Functions
    void JumpPressed();
    void InteractPressed();
    void MagnetPressed();
    void MagnetReleased();
    void PausePressed();
};
