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
auto hitbox = new RectangleComponent(Vector(21, 64), Vector(22, 64));

Player::Player()
{
	AddAnimation(Idle, Animation("resource/img/Knight/Idle.png", 4, 0.17));
	AddAnimation(Run, Animation("resource/img/Knight/Run.png", 7, 0.09));
	AddAnimation(Fall, Animation("resource/img/Knight/Fall.png", 4, 0.135, false));
	AddAnimation(Jump, Animation("resource/img/Knight/Jump.png", 2, 0.12, false));


	Play(Idle);
	anchor = Vector(0.25, 0.75);
	//anchor = Vector(0.25, 0.75);
	//anchor = Vector(0.5, 0.5);
	
	hitbox->layer = Layer::Player;
	hitbox->mask = ToMask(Layer::Ground) | ToMask(Layer::Enemy) | ToMask(Layer::Item);
	hitbox->hasPhysics = true;
	//hitbox->bodyType = BodyType::Dynamic;
	
	auto hitbox_bounds = hitbox->GetBounds();
	auto groundBox = new RectangleComponent(Vector(hitbox_bounds.x+2, hitbox_bounds.y + hitbox_bounds.h), Vector(hitbox_bounds.w/2, 1));
	groundBox->layer = Layer::Sensor;
	groundBox->mask = ToMask(Layer::Ground);
	groundBox->isTrigger = true;	

	rb = new RigidbodyComponent();
	jumpTime = 0.5f;
	jumpHeight = 100.f;
	rb->SetJump(jumpHeight, jumpTime);

	auto atkBox = new RectangleComponent(Vector(hitbox_bounds.x + hitbox_bounds.w, hitbox_bounds.y), Vector(32, hitbox_bounds.y));
	atkBox->layer = Layer::Player;
	atkBox->mask = ToMask(Layer::Ground) | ToMask(Layer::Enemy) | ToMask(Layer::Item);

	Add(hitbox);
	Add(groundBox);
	Add(rb);
	Add(atkBox);
}

void Player::OnUpdate(float dt)
{
	rb->velocity.x = 0;

	//if (!onGround) rb->velocity.y += rb->gravity * dt;

	if (Key[SDL_SCANCODE_J]) {
		angle += 90 * dt;
	}

	if (Key[SDL_SCANCODE_A]) {
		if (rb->velocity.y == 0) Play(Run);
		rb->velocity.x = -speed;
		if (direction != Left) {
			direction = Left;
			HorizontalFlip();
		}
	}
	else if (Key[SDL_SCANCODE_D]) {
		if (rb->velocity.y == 0) Play(Run);
		rb->velocity.x = speed;
		if (direction != Right) {
			direction = Right;
			HorizontalFlip();
		}
	}

	if (Key[SDL_SCANCODE_W]) {
		if (direction != Up) {
			direction = Up;
			VerticalFlip();
		}
	}
	else if (Key[SDL_SCANCODE_S]) {
		if (direction != Down) {
			direction = Down;
			VerticalFlip();
		}
	}

	if (Key[SDL_SCANCODE_SPACE] && onGround) {
		rb->velocity.y = -rb->jump;
		Play(Jump);
	}
	if (rb->velocity.y > 0) Play(Fall);

	if (rb->velocity.x == 0 && rb->velocity.y == 0) Play(Idle);
	//std::cout << "Velocity: " << rb->velocity.x << ", " << rb->velocity.y << std::endl;	

	position += rb->velocity * dt;

	Animation2DComponent::OnUpdate(dt);
}

void Player::OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	if (self->layer == Layer::Sensor && otherShape->layer == Layer::Ground)
	{
		onGround = true;
	}
}

void Player::OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	if (self->layer == Layer::Sensor && otherShape->layer == Layer::Ground)
	{
		onGround = false;
	}
}

void Player::SetSpawnPoint(const Vector& spawnPoint)
{
	position = Vector(spawnPoint.x + hitbox->GetBounds().x / 2, spawnPoint.y + hitbox->GetBounds().w / 2);
}
