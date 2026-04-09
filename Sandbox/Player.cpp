#include "Player.h"
#include "Coin.h"
#include <iostream>

enum State
{
	Idle,
	Walk,
	Run,
	Jump,
	Fall
};

enum Direction
{
	Left,
	Right,
	Up,
	Down
};

Direction direction = Right;
auto rect1 = new RectangleComponent(Vector(21, 64), Vector(22, 64));

Player::Player()
{
	AddAnimation(Idle, Animation("resource/img/Knight/Idle.png", 4, 0.17));
	AddAnimation(Run, Animation("resource/img/Knight/Run.png", 7, 0.09));
	AddAnimation(Fall, Animation("resource/img/Knight/Fall.png", 4, 0.135));
	AddAnimation(Jump, Animation("resource/img/Knight/Jump.png", 2, 0.12));

	Play(Idle);
	position = Vector(100, 100);
	anchor = Vector(0.25, 0.75);
	rect1->layer = Layer::Player;
	rect1->mask = ToMask(Layer::Ground) | ToMask(Layer::Enemy) | ToMask(Layer::Item);
	rect1->hasPhysics = true;
	//anchor = Vector(0.25, 0.75);
	//anchor = Vector(0.5, 0.5);
	

	Add(rect1);
}

void Player::OnUpdate(float dt)
{
	float speed = 300;
	Vector velocity(0, 0);

	if (Key[SDL_SCANCODE_A]) {
		Play(Run);
		velocity.x = -speed;
		if(direction != Left) {
			direction = Left;
			HorizontalFlip();
		}
	} 
	else if (Key[SDL_SCANCODE_D]) {
		Play(Run);
		velocity.x = speed;
		if(direction != Right) {
			direction = Right;
			HorizontalFlip();
		}
	}
	if (Key[SDL_SCANCODE_W]) {
		velocity.y = -speed;
	}
	if (Key[SDL_SCANCODE_S]) {
		velocity.y = speed;
	}
	if (velocity.x == 0) Play(Idle);

	position += velocity * dt;	

	Animation2DComponent::OnUpdate(dt);
}

void Player::OnCollision(ShapeComponent* shape, Component* other)
{

	Rect r = rect1->GetWorldBounds();
	auto shape1 = dynamic_cast<RectangleComponent*>(shape);
	auto other1 = dynamic_cast<Coin*>(other);

	if (shape1)
	{
		Rect otherRect = shape1->GetWorldBounds();
		Vector overlap = r.GetOverlap(otherRect);
		Vector mtv = r.GetMTV(otherRect);
		position.x += mtv.x;
		position.y += mtv.y;
	}
}
