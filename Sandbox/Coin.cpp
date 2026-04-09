#include "Coin.h"
#include <iostream>

Coin::Coin(float x, float y)
{
	position = Vector(x, y);
}	

void Coin::OnLoad()
{
	AddAnimation(0, Animation("resource/img/Item/Coin.png", 5, 0.08));

	
	Add(new RectangleComponent(Vector(0, 0), Vector(16, 16)));
	anchor = Vector(0.5f, 0.5f);
	Play(0);
}

void Coin::OnCollision(ShapeComponent* shape, Component* other)
{
	std::cout << "Coin collected!" << std::endl;
	RemoveFromParent();
}
