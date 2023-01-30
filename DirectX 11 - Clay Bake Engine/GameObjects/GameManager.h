#pragma once
#include "Component.h"
#include "Physics.h"
#include <string>

class GameManager :
    public Component
{
    bool isPaused;
    GameObject* Players[2];
    int activePlayers;
    struct playerInfo 
    {
        Vector2 startLocation;
        //string playerColour;
        enum DirectionOfMagnet
        {
            Push,
            Pull
        };
    };
    void Start();
    void Update();
    void FixedUpdate(float timestamp);
    void LevelWin();
    void LevelLose();
    void LevelReset();
    void Pause();
    void UnPause();
    void GravityFlip();
};

