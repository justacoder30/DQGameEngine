#include "Skeleton.h"
#include "Player.h"
#include <iostream>

Skeleton::Skeleton(const Vector& pos)
{
	hitbox = new RectangleComponent(Vector(40, 16), Vector(16, 48));

	position = Vector(pos.x + hitbox->GetBounds().w / 2, pos.y + hitbox->GetBounds().h / 2);
}

void Skeleton::OnLoad()
{
	AddAnimation(0, Animation("resource/img/Enemy/Skeleton/Walk.png", 10, 0.08));
	Play(0);

	anchor = Vector(0.5, 0.625);

	auto hitbox_bounds = hitbox->GetBounds();

	sensor_ground = new RectangleComponent(Vector(hitbox_bounds.x + hitbox_bounds.w / 4, hitbox_bounds.y + hitbox_bounds.h), Vector(hitbox_bounds.w / 2, 2));
	sensor_ground->layer = Layer::Sensor;
	sensor_ground->mask = ToMask(Layer::Ground);
	sensor_ground->isTrigger = true;

	sensor_wall = new RectangleComponent(Vector(hitbox_bounds.x + hitbox_bounds.w, hitbox_bounds.y + hitbox_bounds.h/4), Vector(2, hitbox_bounds.h/2));
	sensor_wall->layer = Layer::Sensor;
	sensor_wall->mask = ToMask(Layer::Ground);
	sensor_wall	->isTrigger = true;

	sensor_edge = new RectangleComponent(Vector(hitbox_bounds.x + hitbox_bounds.w, hitbox_bounds.y + hitbox_bounds.h), Vector(2, 2));
	sensor_edge->layer = Layer::Sensor;
	sensor_edge->mask = ToMask(Layer::Ground);
	sensor_edge->isTrigger = true;

	hitbox->layer = Layer::Enemy;
	hitbox->mask = ToMask(Layer::Ground) | ToMask(Layer::Attack);

	RigidbodyComponent* rb = new RigidbodyComponent();
	//jumpTime = 0.5f;
	//jumpHeight = 100.f;
	//rb->SetJump(0.5f, 100.f);
	//rb->bodyType = BodyType::Dynamic;

	float jumpTime = 0.25f;
	float jumpHeight = 30.f;

	controller = new CharacterController();
	controller->hitbox = hitbox;
	controller->gravity = (2 * jumpHeight) / (jumpTime * jumpTime);
	controller->jumpForce = 2 * jumpHeight / jumpTime;

	Add(hitbox);
	Add(sensor_ground);
	Add(sensor_wall);
	Add(sensor_edge);
	Add(rb);
	Add(controller);
	controller->velocity.x = speed;
}

void Skeleton::OnUpdate(float dt)
{
	if (!sensor_edge->isColliding) {
		if (onGround) {
			speed *= -1;
			HorizontalFlip();
		}
	}
	if (onGround) controller->velocity.x = speed;
	else controller->velocity.x = 0;
	if (!onGround) controller->velocity.y += controller->gravity * dt;

	//if (controller->velocity.x != 0) controller->MoveX(controller->velocity.x * dt);
	//if (controller->velocity.y != 0) controller->MoveY(controller->velocity.y * dt);

	Animation2DComponent::OnUpdate(dt);
}

void Skeleton::OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	if (self == sensor_ground && otherShape->layer == Layer::Ground)
	{
		onGround = true;
	}
	else if (self == sensor_wall && otherShape->layer == Layer::Ground)
	{
		if (onGround) {
			speed *= -1;
			HorizontalFlip();
		}
	}

	if (dynamic_cast<Player*>(other) && otherShape->layer == Layer::Attack) {
		std::cout << "Player Hit" << std::endl;
		controller->velocity.y = -controller->jumpForce;
		onGround = false;
	}
}

void Skeleton::OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	if (self == sensor_ground && otherShape->layer == Layer::Ground)
	{
		onGround = true;
	}
}

void Skeleton::OnCollisionEnd(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	if (self == sensor_ground && otherShape->layer == Layer::Ground)
	{
		onGround = false;
	}
}
