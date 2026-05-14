#pragma once

class Time
{
public:
    static float timeScale;

    static float freezeTimer;

    static void Freeze(float duration);

    static void Update(float dt);
};