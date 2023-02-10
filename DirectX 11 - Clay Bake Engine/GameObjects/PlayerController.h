#pragma once
#include "Components/Component.h"
#include "Components/Rigidbody.h"
#include "../Input/PlayerInput.h"
#include "../Physics/Vectors.h"
#include "../Audio/SoundEffect.h"
#include "Components/Interactable.h"
#include "GameObject.h"
#include "GameManager.h"
#include "ObjectHandler.h"

class PlayerMagnetism;

enum FacingDirection
{
    Left,
    Right
};

enum PlayerState
{
    IdleState,
    WalkState,
    JumpState,
    FallState,
    MagnetState
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
    Transform* _playerTransform;
    Appearance* _playerAppearance;
    SoundEffect* _jumpSoundEffect;
    SoundEffect* _moveSoundEffect;
    PlayerMagnetism* _magnet;

    int _playerID;
    float _moveSpeed = 150.0f;
    float _topSpeed = 10000.0f;
    float _jumpTimer = 1.0f;
    float _activeJumpTimer = 0.0f;
    float _animationFrameDelay = 0.1f;
    float _activeFrameDelay = 0.0f;
    int _currentFrame = 0;

    bool _jumpReset = true;
    bool _movementEnabled = true;
    bool _isWalking = false;
    bool _isJumping = false;

    Vector2 _interactArea = Vector2{ 50.0f , 50.0f };
    Vector2 _currentMovement;
    Vector2 _jumpForce = {0.0f, 25000.0f};
    FacingDirection _facingDirection = Right;
    PlayerState _playerState = IdleState;

    //Functions
    void JumpPressed();
    void InteractPressed();

    bool CheckForCollisionsBelowDirect();
};
