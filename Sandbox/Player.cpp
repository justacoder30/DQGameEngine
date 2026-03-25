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

Player::Player()
{
	/*AddAnimation(Idle, Animation("resource/img/Knight/Idle.png", 4, 0.17));
	AddAnimation(Run, Animation("resource/img/Knight/Run.png", 7, 0.09));
	AddAnimation(Fall, Animation("resource/img/Knight/Fall.png", 4, 0.135));
	AddAnimation(Jump, Animation("resource/img/Knight/Jump.png", 2, 0.12));

	Play(Idle);

	SetPosition(900, 400);

	SetSize(400, 400);*/

	sprite1 = new SpriteComponent(new Texture("resource/img/Knight/Run.png"));
	Add(sprite1);
}

void Player::OnUpdate(float dt)
{
	float speed = 300;

	if (Key[SDL_SCANCODE_A]) {
		//Play(Run);
		sprite1->x -= speed * dt;
		if(direction != Left) {
			direction = Left;
			animationClip.flip = true; 
		}
	} 
	else if (Key[SDL_SCANCODE_D]) {
		//Play(Run);
		sprite1->x += speed * dt;
		if(direction != Right) {
			direction = Right;
			animationClip.flip = false; 
		}
	}
	else if (Key[SDL_SCANCODE_W]) {
		sprite1->y -= speed * dt;
	}
	else if (Key[SDL_SCANCODE_S]) {
		sprite1->y += speed * dt;
	}
	else {
		//Play(Idle);
	}

	//Animation2DComponent::OnUpdate(dt);
}
