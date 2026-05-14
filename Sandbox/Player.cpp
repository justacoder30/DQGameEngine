#include "Player.h"
#include "Skeleton.h"
#include "Coin.h"

#include <iostream>

Player::Player()
{
	jumpTime = 0.5f;
	jumpHeight = 100.f;

	std::vector<std::string> idleAnimation = CreateStringAnimate("adventurer-idle-2-0", 4);
	std::vector<std::string> runAnimation = CreateStringAnimate("adventurer-run-0", 6);
	std::vector<std::string> jumpAnimation = CreateStringAnimate("adventurer-jump-0", 4);
	std::vector<std::string> fallAnimation = CreateStringAnimate("adventurer-fall-0", 2);
	std::vector<std::string> rollAnimation = CreateStringAnimate("adventurer-smrslt-0", 4);
	std::vector<std::string> atk1Animation = CreateStringAnimate("adventurer-attack1-0", 5);
	std::vector<std::string> atk2Animation = CreateStringAnimate("adventurer-attack2-0", 6);
	std::vector<std::string> atk3Animation = CreateStringAnimate("adventurer-attack3-0", 6);
	std::vector<std::string> hurtAnimation = CreateStringAnimate("adventurer-hurt-0", 3);
	std::vector<std::string> deathAnimation = CreateStringAnimate("adventurer-die-0", 7);

	std::vector<std::string> airAtk1Animation = CreateStringAnimate("adventurer-air-attack1-0", 4);
	std::vector<std::string> airAtk2Animation = CreateStringAnimate("adventurer-air-attack2-0", 3);
	std::vector<std::string> airAtk3Animation = CreateStringAnimate("adventurer-air-attack3-loop-0", 2);
	std::vector<std::string> airAtkEndAnimation = CreateStringAnimate("adventurer-air-attack-3-end-0", 3);

	AddAnimation(Idle, Animation(idleAnimation, 0.15));
	AddAnimation(Run, Animation(runAnimation, 0.1));
	AddAnimation(Attack1, Animation(atk1Animation, attacks[0].duration / atk1Animation.size(), false));
	AddAnimation(Attack2, Animation(atk2Animation, attacks[1].duration / atk2Animation.size(), false));
	AddAnimation(Attack3, Animation(atk3Animation, attacks[2].duration / atk3Animation.size(), false));

	AddAnimation(AirAttack1, Animation(airAtk1Animation, airAttacks[0].duration / airAtk1Animation.size(), false));
	AddAnimation(AirAttack2, Animation(airAtk2Animation, airAttacks[1].duration / airAtk2Animation.size(), false));
	AddAnimation(AirAttack3, Animation(airAtk3Animation, airAttacks[2].duration / airAtk3Animation.size()));
	AddAnimation(AirAttackEnd, Animation(airAtkEndAnimation, airAttacks[3].duration / airAtkEndAnimation.size(), false));

	AddAnimation(Jump, Animation(jumpAnimation, jumpTime / jumpAnimation.size(), false));
	AddAnimation(Fall, Animation(fallAnimation, 0.12));
	AddAnimation(Roll, Animation(rollAnimation, 0.12, false));
	AddAnimation(Hurt, Animation(hurtAnimation, 0.12, false));
	AddAnimation(Death, Animation(deathAnimation, 0.12, false));

	//Play(Idle);

	state = new StateMachineComponent();

	idleState = new IdleState(this);
	runState = new RunState(this);
	jumpState = new JumpState(this);
	fallState = new FallState(this);
	attackState = new AttackState(this);
	airAttackState = new AirAttackState(this);
	hurtState = new HurtState(this);
	deathState = new DeathState(this);
	airAttackEndkState = new AirAttackEndState(this);
	rollState = new RollState(this);

	state->ChangeState(idleState);


	float scale = 2;

	SetSize(50 * scale, 37 * scale);

	anchor = Vector(0.5, 0.5);

	hitbox = new RectangleComponent(Vector(20 * scale, 9 * scale), Vector(13 * scale, 28 * scale));
	hitbox->layer = Layer::Player;
	hitbox->mask = ToMask(Layer::Ground) | ToMask(Layer::Enemy) | ToMask(Layer::Item) | ToMask(Layer::Attack);
	hitbox->hasPhysics = true;
	hitbox->bodyType = BodyType::Dynamic;

	rollHitbox = new RectangleComponent(Vector(20 * scale, 9 * scale), Vector(13 * scale, 19 * scale));
	rollHitbox->layer = Layer::Player;
	rollHitbox->mask = ToMask(Layer::Ground) | ToMask(Layer::Enemy) | ToMask(Layer::Item) | ToMask(Layer::Attack);
	rollHitbox->hasPhysics = true;
	rollHitbox->bodyType = BodyType::Dynamic;

	auto rollHitbox_bounds = rollHitbox->GetBounds();	
	rollGroundBox = new RectangleComponent(Vector(rollHitbox_bounds.x, rollHitbox_bounds.y + rollHitbox_bounds.h), Vector(rollHitbox_bounds.w, 2));
	rollGroundBox->layer = Layer::Sensor;
	rollGroundBox->mask = ToMask(Layer::Ground);
	rollGroundBox->isTrigger = true;

	auto hitbox_bounds = hitbox->GetBounds();
	groundBox = new RectangleComponent(Vector(hitbox_bounds.x, hitbox_bounds.y + hitbox_bounds.h), Vector(hitbox_bounds.w, 2));
	groundBox->layer = Layer::Sensor;
	groundBox->mask = ToMask(Layer::Ground);
	groundBox->isTrigger = true;

	jumpTime = 0.5f;
	jumpHeight = 100.f;

	atkBox = new RectangleComponent(Vector(hitbox_bounds.x + hitbox_bounds.w, hitbox_bounds.y), Vector(19 * scale, 28 * scale));
	atkBox->layer = Layer::Attack;
	atkBox->mask = ToMask(Layer::Enemy);
	atkBox->bodyType = BodyType::NoneType;

	atkEndBox = new RectangleComponent();
	atkEndBox->layer = Layer::Attack;
	atkEndBox->mask = ToMask(Layer::Enemy);
	atkEndBox->bodyType = BodyType::NoneType;


	controller = new CharacterController();
	controller->hitbox = hitbox;
	controller->gravity = (2 * jumpHeight) / (jumpTime * jumpTime);
	controller->jumpForce = 2 * jumpHeight / jumpTime;

	CharacterController::StepSize = 4;

	Add(state);

	Add(hitbox);
	Add(rollHitbox);
	Add(rollGroundBox);
	Add(groundBox);
	Add(atkBox);
	Add(atkEndBox);
	Add(controller);

	//DebugMode = true;

	//healthbar = new Healthbar(Vector(15, 5), Vector(60, 5));
	//Add(healthbar);

	
}

void Player::OnLoad()
{
	

	//SetColor({ 1, 0, 0, 1 });
	Animation2DComponent::OnLoad();
}

void Player::OnUpdate(float dt)
{
	healthbar->Show();
	atkBox->active = false;
	atkEndBox->active = false;
	controller->velocity.x = 0;

	if (!onGround) controller->velocity.y += controller->gravity * dt;

	//if (Key[SDL_SCANCODE_J]) atkBox->active = true;

	if (Key[SDL_SCANCODE_J] && !PreKey[SDL_SCANCODE_J])
		attackBuffer = 0.1f;

	attackBuffer -= dt;

	if (Key[SDL_SCANCODE_A]) {
		//if (controller->velocity.y == 0) Play(Run);
		controller->velocity.x = -speed;
		if (direction != Left) {
			direction = Left;
			HorizontalFlip();
		}
	}
	else if (Key[SDL_SCANCODE_D]) {
		//if (controller->velocity.y == 0) Play(Run);
		controller->velocity.x = speed;
		if (direction != Right) {
			direction = Right;
			HorizontalFlip();
		}
	}

	if (Key[SDL_SCANCODE_LEFT] && !PreKey[SDL_SCANCODE_LEFT]) {
		//if (controller->velocity.y == 0) Play(Run);
		controller->velocity.x = -100000;
		if (direction != Left) {
			direction = Left;
			HorizontalFlip();
		}
	}
	else if (Key[SDL_SCANCODE_RIGHT] && !PreKey[SDL_SCANCODE_RIGHT]) {
		//if (controller->velocity.y == 0) Play(Run);
		controller->velocity.x = 100000;
		if (direction != Right) {
			direction = Right;
			HorizontalFlip();
		}
	}

	if (Key[SDL_SCANCODE_SPACE] && !PreKey[SDL_SCANCODE_SPACE] && onGround) {
		controller->velocity.y = -controller->jumpForce;
		//Play(Jump);
	}

	Animation2DComponent::OnUpdate(dt);
}


void Player::OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	if (self->layer == Layer::Sensor && otherShape->layer == Layer::Ground)
	{
		onGround = true;
	}
	else if (self == hitbox && otherShape->layer == Layer::Attack && !invincible)
	{
		auto skeleton = dynamic_cast<Skeleton*>(other);
		if (skeleton) {
			Time::Freeze(0.03f);
			Renderer2D::GetCamera()->Shake(1.f, 0.1f);
			hp -= skeleton->atkDamage;
			healthbar->SetHealth(hp , MaxHP);
			if (hp <= 0) state->ChangeState(deathState);
			else state->ChangeState(hurtState);
		}
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
	this->spawnPoint = position;
	std::cout << "Player Spawn Point: " << this->spawnPoint.x << ", " << this->spawnPoint.y << std::endl;
}

void Player::ReSpawn()
{
	position = spawnPoint;
	hp = MaxHP;
	state->ChangeState(idleState);
	healthbar->SetHealth(hp, MaxHP);

	
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
