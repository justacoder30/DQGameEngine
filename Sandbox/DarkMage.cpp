#include "DarkMage.h"
#include "Skeleton.h"
#include <iostream>

DarkMage::DarkMage(const Vector& pos)
{
	hitbox = new RectangleComponent(Vector(72 * scale, 69 * scale), Vector(16 * scale, 46 * scale));

	position = Vector(pos.x + hitbox->GetBounds().w / 2, pos.y + hitbox->GetBounds().h / 2);
}

void DarkMage::OnLoad()
{
	AddAnimation(Idle, Animation("resource/img/Enemy/DarkMage/DarkMage.png", 8, 0.08, true, 0, 7, Vector(160, 128)));
	AddAnimation(Run, Animation("resource/img/Enemy/DarkMage/DarkMage.png", 8, 0.08, true, 1, 7, Vector(160, 128)));
	//AddAnimation(Attack2, Animation("resource/img/Enemy/DarkMage./DarkMage.png", 13, 0.08, false, 2, 7, Vector(160, 128)));
	//AddAnimation(Heal, Animation("resource/img/Enemy/DarkMage/DarkMage.png", 8, 0.08, false, 3, 7, Vector(160, 128)));
	AddAnimation(Attack1, Animation("resource/img/Enemy/DarkMage/DarkMage.png", 17, 0.08, false, 2, 13, Vector(160, 128)));
	AddAnimation(Hurt, Animation("resource/img/Enemy/DarkMage/DarkMage.png", 5, 0.08, false, 5, 7, Vector(160, 128)));
	AddAnimation(Death, Animation("resource/img/Enemy/DarkMage/DarkMage.png", 9, 0.08, false, 6, 7, Vector(160, 128)));

	healthbar = new Healthbar(Vector(500, 10), Vector(300, 15));
	healthbar->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	healthbar->layer = RenderLayer::UI;

	//hitbox = new RectangleComponent(Vector(72 * scale, 69 * scale), Vector(16 * scale, 46 * scale));
	hitbox->layer = Layer::Enemy;
	hitbox->mask = ToMask(Layer::Ground) | ToMask(Layer::Attack);

	sensor_player = new RectangleComponent(Vector(0, 0), Vector(300, 300));
	sensor_player->layer = Layer::Enemy;
	sensor_player->mask = ToMask(Layer::Player);
	sensor_player->bodyType = BodyType::NoneType;
	sensor_player->isTrigger = true;

	float jumpTime = 0.25f;
	float jumpHeight = 30.f;

	controller = new CharacterController();
	controller->hitbox = hitbox;
	controller->gravity = (2 * jumpHeight) / (jumpTime * jumpTime);
	controller->jumpForce = 2 * jumpHeight / jumpTime;

	state = new StateMachineComponent();

	SetSize(160 * scale, 128 * scale);
	
	SetAnchor(0.625f, 0.625f);

	Add(healthbar);
	Add(state);
	Add(sensor_player);
	Add(hitbox);
	Add(controller);

	idleState = new DarkMageIdleState(this);
	chaseState = new DarkMageChaseState(this);
	attack1State = new DarkMageAttackState(this);
	hurtState = new DarkMageHurtState(this);
	deathState = new DarkMageDeathState(this);

	state->ChangeState(idleState);

	Animation2DComponent::OnLoad();
}

void DarkMage::OnUpdate(float dt)
{
	if (target) healthbar->Show();

	Animation2DComponent::OnUpdate(dt);
}

void DarkMage::OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	auto player = dynamic_cast<Player*>(other);

	if (player && otherShape->layer == Layer::Attack)
	{
		if (dead) return;

		hp -= player->atkDamage;
		Time::Freeze(0.04f);
		Renderer2D::GetCamera()->Shake(1.2f, 0.1f);
		GetComponent<Healthbar>()->SetHealth(hp, MaxHP);

		state->ChangeState(hurtState);
	}
}

void DarkMage::OnCollision(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
	if (self == sensor_player && otherShape->layer == Layer::Player)
	{
		auto player = dynamic_cast<Player*>(other);
		if (player) {
			target = static_cast<Player*>(other);
		}
	}
}

void DarkMage::SpawnMinion()
{
	static int cnt = 5;
	if (cnt <= 0) {
		maxMinions = true;
		return;
	}
	cnt--;
	Vector spawnPos = position;

	spawnPos.x += RandomFloat(-120.f, 120.f);

	auto skeleton = new Skeleton(spawnPos);
	static Skeleton* lastSkeleton = skeleton;
	std::cout << "Onground: "	 << lastSkeleton->onGround << std::endl;

	GetParent()->Add(skeleton);
}
