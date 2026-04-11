#pragma once
#include "Components\SpriteComponent.h"
#include "Components\RectangleComponent.h"
#include "Core\CollisionCallbacks.h"
#include "Coin.h"
#include <iostream>

class Background : public SpriteComponent
{

public:
	Background() : SpriteComponent("resource/img/Background/background.png") {
		size = Renderer2D::GetViewportSize();

		position = Vector(0, 0);
		anchor = Vector(1.0f, 1.0f);
		anchor = Vector(0.5f, 0.5f);
		//anchor = Vector(1, 0.5f);
		layer = RenderLayer::Background;

		auto rect = new RectangleComponent(Vector(700, 200), Vector(100, 100));
		rect->layer = Layer::Ground;
		rect->mask = ToMask(Layer::Player);

		Add(rect);

		coin = new Coin(0, 0);
		coin->layer = RenderLayer::UI;
		//coin->position = Vector(-784/2, -441/2);
		Add(coin);
		
	}

	Coin* coin;
};

