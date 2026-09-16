#include "pch.h"
#include "Time.h"

float Time::timeScale = 1.0f;
float Time::freezeTimer = 0;
//srand((unsigned int)time(nullptr));

void Time::Freeze(float duration)
{
    freezeTimer = duration;
    timeScale = 0.0f;
}

void Time::Update(float dt)
{
    if (freezeTimer > 0)
    {
        freezeTimer -= dt;

        if (freezeTimer <= 0)
        {
            timeScale = 1.0f;
        }
    }
}