#include "Player.h"
#include "Coin.h"
#include <iostream>

enum State
{
	Idle,
	Walk,
	Run,
	Jump,
	Fall,
	Attack1,
};

enum Direction
{
	Left,
	Right,
	Up,
	Down
};

Direction direction = Right;


Player::Player()
{
	jumpTime = 0.5f;
	jumpHeight = 100.f;

	std::vector<std::string> idleAnimation = CreateStringAnimate("adventurer-idle-2-0", 4);
	std::vector<std::string> runAnimation = CreateStringAnimate("adventurer-run-0", 6);
	std::vector<std::string> jumpAnimation = CreateStringAnimate("adventurer-jump-0", 4);
	std::vector<std::string> fallAnimation = CreateStringAnimate("adventurer-fall-0", 2);
	std::vector<std::string> atk1Animation = CreateStringAnimate("adventurer-attack1-0", 4);

	AddAnimation(Idle, Animation(idleAnimation, 0.15));
	AddAnimation(Run, Animation(runAnimation, 0.1));
	AddAnimation(Attack1, Animation(atk1Animation, 0.1));
	AddAnimation(Jump, Animation(jumpAnimation, jumpTime/ jumpAnimation.size(), false));
	AddAnimation(Fall, Animation(fallAnimation, 0.12));

	Play(Idle);
	float scale = 2;

	SetSize(50 * scale, 37 * scale);

	anchor = Vector(0.5, 0.5);

	hitbox = new RectangleComponent(Vector(20 * scale, 9 * scale), Vector(13 * scale, 28 * scale));
	hitbox->layer = Layer::Player;
	hitbox->mask = ToMask(Layer::Ground) | ToMask(Layer::Enemy) | ToMask(Layer::Item);
	hitbox->hasPhysics = true;
	hitbox->bodyType = BodyType::Dynamic;

	auto hitbox_bounds = hitbox->GetBounds();
	auto groundBox = new RectangleComponent(Vector(hitbox_bounds.x + hitbox_bounds.w / 4, hitbox_bounds.y + hitbox_bounds.h), Vector(hitbox_bounds.w / 2, 2));
	groundBox->layer = Layer::Sensor;
	groundBox->mask = ToMask(Layer::Ground);
	groundBox->isTrigger = true;

	jumpTime = 0.5f;
	jumpHeight = 100.f;

	auto atkBox = new RectangleComponent(Vector(hitbox_bounds.x + hitbox_bounds.w, hitbox_bounds.y), Vector(19 * scale, 28 * scale));
	atkBox->layer = Layer::Attack;
	atkBox->mask = ToMask(Layer::Ground) | ToMask(Layer::Enemy) | ToMask(Layer::Item);
	atkBox->bodyType = BodyType::NoneType;

	controller = new CharacterController();
	controller->hitbox = hitbox;
	controller->gravity = (2 * jumpHeight) / (jumpTime * jumpTime);
	controller->jumpForce = 2 * jumpHeight / jumpTime;

	Add(controller);

	Add(hitbox);
	Add(groundBox);
	Add(atkBox);
}

void Player::OnUpdate(float dt)
{
	controller->velocity.x = 0;

	if (!onGround) controller->velocity.y += controller->gravity * dt;

	if (Key[SDL_SCANCODE_A]) {
		if (controller->velocity.y == 0) Play(Run);
		controller->velocity.x = -speed;
		if (direction != Left) {
			direction = Left;
			HorizontalFlip();
		}
	}
	else if (Key[SDL_SCANCODE_D]) {
		if (controller->velocity.y == 0) Play(Run);
		controller->velocity.x = speed;
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
		controller->velocity.y = -controller->jumpForce;
		Play(Jump);
	}
	if (controller->velocity.y > 0) Play(Fall);

	if (controller->velocity.x == 0 && controller->velocity.y == 0) Play(Idle);
	//std::cout << "Velocity: " << rb->velocity.x << ", " << rb->velocity.y << std::endl;	

	//position += controller->velocity * dt;

	//if (controller->velocity.x != 0) controller->MoveX(controller->velocity.x * dt);
	//if (controller->velocity.y != 0) controller->MoveY(controller->velocity.y * dt);

	Animation2DComponent::OnUpdate(dt);
}


void Player::OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	if (self->layer == Layer::Sensor && otherShape->layer == Layer::Ground)
	{
		onGround = true;
	}
}

void Player::OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
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

std::vector<std::string> Player::CreateStringAnimate(const std::string& f, int cout)
{
	std::vector<std::string> animations;

	for (size_t i = 0; i < cout; i++)
	{
		animations.push_back("resource/img/Hero/" + f + std::to_string(i) + ".png");
	}

	return animations;
}
