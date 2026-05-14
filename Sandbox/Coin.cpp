#include "Coin.h"
#include "Player.h"
#include <iostream>



Coin::Coin(float x, float y)
{
	position = Vector(x, y);
}	

void Coin::OnLoad()
{
	AddAnimation(0, Animation("resource/img/Item/Coin.png", 5, 0.08));

	auto r = new RectangleComponent(Vector(0, 0), Vector(16, 16));
	r->layer = Layer::Item;	
	r->mask = ToMask(Layer::Player);
	//r->hasPhysics = true;
	//r->bodyType = BodyType::Dynamic;

	Add(r);
	Play(0);

	Animation2DComponent::OnLoad();
}

void Coin::OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	if (dynamic_cast<Player*>(other))
	{
		RemoveFromParent();
	}
}
