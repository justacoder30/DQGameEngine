#pragma once
#include "Components/ShapeComponent.h"  

class ShapeComponent;
class Component;

class CollisionCallbacks
{
public:
    virtual void OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other) {}
    virtual void OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other) {}
    virtual void OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other) {}
};

