#pragma once
#include "Components\SpriteComponent.h"
#include "Components\RectangleComponent.h"
#include "Core\CollisionCallbacks.h"
#include <iostream>

class Background : public SpriteComponent, public CollisionCallbacks
{

public:
	Background() : SpriteComponent(new Texture("resource/img/Background/background.png")) {
		size.x = 1920;   
		size.y = 1080;

		auto rect = new RectangleComponent(Vector(700, 200), Vector(100, 100));
		auto rect1 = new RectangleComponent(Vector(700, 100), Vector(100, 100));

		Add(rect);
		Add(rect1);
	}

	void OnCollision(ShapeComponent* shape, Component* other) override
	{
		RemoveFromParent();
	}
};

