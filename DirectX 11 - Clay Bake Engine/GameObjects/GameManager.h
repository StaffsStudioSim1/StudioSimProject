#pragma once
//#include "Component.h"
#include "Physics.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class GameManager
{
    bool isPaused;
    bool isGravityFlipped;
    GameObject* Players[2];
    vector <int> levelList;
    
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
public:
    void Start();
    void Update();
    void FixedUpdate(float timestamp);
    void LevelWin();
    void LevelLose();
    void LevelReset();
    void Pause();
    void UnPause();
    void GravityFlip();
    void GravityFlipOn();
    void GravityFlipOff();
    void SaveFileCheck();
    void CreateSaveFile();
};

