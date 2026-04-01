#pragma once
#include "Components\SpriteComponent.h"
#include "Components\RectangleComponent.h"
class Background : public SpriteComponent
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
};

