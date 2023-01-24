#pragma once
#include "Component.h"
class GameManager :
    public Component
{
    void Start();
    void Update();
    void FixedUpdate(float timestamp);
    void OnPlayerJoined();
    void LevelWin();
    void LevelLose();
    void LevelReset();
};

