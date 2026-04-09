#pragma once
#include "Components/ShapeComponent.h"  

class ShapeComponent;
class Component;

class CollisionCallbacks
{
public:
    virtual void OnCollisionStart(ShapeComponent* shape, Component* other) {}
    virtual void OnCollision(ShapeComponent* shape, Component* other) {}
    virtual void OnCollisionEnd(ShapeComponent* shape, Component* other) {}
};

