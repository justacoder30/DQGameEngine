#include "Player.h"
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
	Right
};

Direction direction = Right;
auto rect1 = new RectangleComponent(Vector(0, 0), Vector(100, 100));

Player::Player()
{
	AddAnimation(Idle, Animation("resource/img/Knight/Idle.png", 4, 0.17));
	AddAnimation(Run, Animation("resource/img/Knight/Run.png", 7, 0.09));
	AddAnimation(Fall, Animation("resource/img/Knight/Fall.png", 4, 0.135));
	AddAnimation(Jump, Animation("resource/img/Knight/Jump.png", 2, 0.12));

	Play(Idle);

	SetPosition(900, 400);

	SetSize(400, 400);
	
	//auto rect2 = new RectangleComponent(Vector(200, 0), Vector(100, 100));

	Add(rect1);
	//Add(rect2);
}

void Player::OnUpdate(float dt)
{
	float speed = 300;
	Vector velocity(0, 0);

	if (Key[SDL_SCANCODE_A]) {
		//Play(Run);
		velocity.x = -speed;
		if(direction != Left) {
			direction = Left;
			animationClip.flip = true; 
		}
	} 
	else if (Key[SDL_SCANCODE_D]) {
		//Play(Run);
		velocity.x = speed;
		if(direction != Right) {
			direction = Right;
			animationClip.flip = false; 
		}
	}
	if (Key[SDL_SCANCODE_W]) {
		velocity.y = -speed;
	}
	if (Key[SDL_SCANCODE_S]) {
		velocity.y = speed;
	}
	else {
		Play(Idle);
	}
	Play(Idle);

	if (rect1->isColliding) velocity = Vector(0, 0);
	position += velocity * dt;	

	Animation2DComponent::OnUpdate(dt);
}

void Player::OnCollision(ShapeComponent* shape, Component* other)
{
	Rect r = rect1->GetWorldBounds();
	auto shape1 = dynamic_cast<RectangleComponent*>(shape);

	if (shape1)
	{
		Rect otherRect = shape1->GetWorldBounds();
		Vector overlap = r.GetOverlap(otherRect);
		std::cout << "Overlap: " << overlap.x << ", " << overlap.y << std::endl;
		Vector mtv = r.GetMTV(otherRect);
		position.x += mtv.x;
		position.y += mtv.y;
	}
}
