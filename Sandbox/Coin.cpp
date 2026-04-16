#include "Coin.h"
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
	r->mask = ToMask(Layer::Player) | ToMask(Layer::Item);
	//r->hasPhysics = true;
	auto rb = new RigidbodyComponent();
	auto controller = new CharacterController();
	controller->collider = r;
	rb->bodyType = BodyType::Dynamic;

	Add(r);
	Add(rb);
	Add(controller);
	Play(0);
}
