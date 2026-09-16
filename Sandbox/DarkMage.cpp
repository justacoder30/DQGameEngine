#include "DarkMage.h"
#include "Skeleton.h"
#include "Head.h"
#include <algorithm>
#include <cmath>

DarkMage::DarkMage(const Vector& pos) : home(pos)
{
    hitbox = new RectangleComponent(Vector(72 * scale, 69 * scale), Vector(16 * scale, 46 * scale));
    position = Vector(pos.x + 16.f, pos.y + 46.f);
    home = position;
}

DarkMage::~DarkMage() = default;

void DarkMage::OnLoad()
{
    const char* sheet = "resource/img/Enemy/DarkMage/DarkMage.png";

    AddAnimation(Idle, Animation(sheet, 8, 0, Vector(160,128), 0.09f, true));
    AddAnimation(Run, Animation(sheet, 8, 1, Vector(160,128), 0.08f, true));
    AddAnimation(Attack1, Animation(sheet, 13, 2, Vector(160,128), 0.10f, false));
    AddAnimation(Attack2, Animation(sheet, 13, 3, Vector(160,128), 0.10f, false));
    AddAnimation(Heal, Animation(sheet, 13, 4, Vector(160,128), 0.12f, false));
    AddAnimation(Hurt, Animation(sheet, 5, 5, Vector(160,128), 0.07f, false));
    AddAnimation(Death, Animation(sheet, 9, 6, Vector(160,128), 0.10f, false));


    SetSize(160 * scale, 128 * scale);
    SetAnchor(0.625f, 0.625f);

    hitbox->layer = Layer::Enemy;
    hitbox->mask = ToMask(Layer::Ground) | ToMask(Layer::Attack);
    sensor_player = new RectangleComponent(Vector(80*scale-detectRange, 90*scale-detectRange), Vector(detectRange*2, detectRange*2));
    sensor_player->layer = Layer::Enemy;
    sensor_player->mask = ToMask(Layer::Player);
    sensor_player->bodyType = BodyType::NoneType;
    sensor_player->isTrigger = true;
    controller = new CharacterController();
    controller->hitbox = hitbox;
    controller->gravity = 960.f;
    controller->jumpForce = 240.f;

    state = new StateMachineComponent();
    healthbar = new Healthbar(Vector(267, 18), Vector(250, 12));
    healthbar->SetColor({0.65f,0.15f,0.9f,1.f});
    healthbar->layer = RenderLayer::UI;

    Add(healthbar); 
    Add(hitbox); 
    Add(sensor_player); 
    Add(state); 
    Add(controller);

    idleState = state->CreateState<DarkMageIdleState>(this);
    chaseState = state->CreateState<DarkMageChaseState>(this);
    attack1State = state->CreateState<DarkMageAttackState>(this);
    attack2State = state->CreateState<DarkMageAttack2State>(this);
    healState = state->CreateState<DarkMageHealState>(this);
    hurtState = state->CreateState<DarkMageHurtState>(this);
    deathState = state->CreateState<DarkMageDeathState>(this);

    state->ChangeState(idleState);

    Animation2DComponent::OnLoad();
}

void DarkMage::OnUpdate(float dt)
{
    attackTimer = std::max(0.f, attackTimer-dt);
    summonTimer = std::max(0.f, summonTimer-dt);
    healTimer = std::max(0.f, healTimer-dt);
    damageGrace = std::max(0.f, damageGrace-dt);
    staggerTimer = std::max(0.f, staggerTimer-dt);
    headVolley->hitGrace = std::max(0.f, headVolley->hitGrace-dt);
    // Resolve the player from the live scene instead of keeping a stale pointer.

    Player* player = nullptr;

    if (GetParent()) {
        for (const auto& owned : GetParent()->GetChildren())
            if (auto* candidate = dynamic_cast<Player*>(owned.get())) {
                player = candidate;
                break;
            }
    }
    const bool wasEngaged = target != nullptr;
    target = nullptr;

    if (!dead && player && player->hp > 0) {

        const float dx = player->position.x-home.x, dy = player->position.y-home.y;
        const float range = wasEngaged ? 800.f : detectRange;

        if (dx*dx+dy*dy < range*range) 
            target = player;
    }

    if (wasEngaged && !target && !dead) {
        ClearAttacks();
        state->ChangeState(idleState);
        attackTimer = 1.f;
    }

    if (HasTarget()) healthbar->Show();
    if (!dead) controller->velocity.y = std::min(controller->velocity.y+controller->gravity*dt, 650.f);
    SetColor(healing ? Color{0.45f,1.f,0.6f,1.f} : damageGrace > 0 ? Color{1.f,0.45f,0.45f,1.f} : Enraged() ? Color{1.f,0.75f,0.85f,1.f} : Color{1,1,1,1});
    Animation2DComponent::OnUpdate(dt);
}

void DarkMage::FaceTarget()
{
    if (!HasTarget()) return;

    const bool right = target->position.x > position.x;

    if (right != facingRight) HorizontalFlip();
    facingRight = right;

    direction = right ? Right : Left;
}

bool DarkMage::CanMove(float dir) const
{
    if (std::abs(position.x+dir*20.f-home.x) > 350.f) 
        return false;

    const Rect feet = hitbox->GetWorldBounds();

    const float x = feet.x+feet.w*0.5f+dir*(feet.w*0.5f+16.f);

    for (auto* shape : GameApp::GetBoardphase()->GetColliders()) {

        if (!shape->active || shape->layer != Layer::Ground) 
            continue;

        auto* ground = dynamic_cast<RectangleComponent*>(shape);

        if (!ground) continue;

        const Rect r = ground->GetWorldBounds();

        if (x >= r.x && 
            x <= r.x+r.w && 
            r.y >= feet.y+feet.h-8.f &&
            r.y <= feet.y+feet.h+28.f
        ) 
            return true;
    }
    return false;
}

bool DarkMage::ChooseAction()
{
    if (!HasTarget() || attackTimer > 0) 
        return false;

    FaceTarget();

    if (hp <= MaxHP*0.45f && healsRemaining > 0 && healTimer <= 0) {
        state->ChangeState(healState); 
        return true;
    }

    if (summonsRemaining > 0 && castsSinceSummon >= 2 && summonTimer <= 0) {
        state->ChangeState(attack1State); return true;
    }

    if (std::abs(target->position.x-position.x) <= detectRange) {
        state->ChangeState(attack2State); return true;
    }

    return false;
}

void DarkMage::FinishCast()
{
    casting = healing = false;
    attackTimer = Enraged() ? 1.0f : attackCooldown;
    state->ChangeState(idleState);
}

void DarkMage::OnCollisionStart(ShapeComponent* self, ShapeComponent* otherShape, Component* other)
{
    auto* player = dynamic_cast<Player*>(other);

    if (self != hitbox || 
        !player || 
        player->hp <= 0 || 
        otherShape->layer != Layer::Attack || 
        dead || 
        damageGrace > 0
    ) 
        return;
    hp = std::max(0, hp-static_cast<int>(player->atkDamage));
    damageGrace = 0.16f;
    healthbar->SetHealth(hp, MaxHP);
    Time::Freeze(0.035f);

    if (auto* camera = Renderer2D::GetCamera()) 
        camera->Shake(1.2f,0.1f);
    if (hp == 0) 
        state->ChangeState(deathState);
    else if (healing || (!casting && staggerTimer <= 0)) {
        // Damage still lands during this window; only repeated flinching is suppressed.
        staggerTimer = 1.2f;
        state->ChangeState(hurtState);
    }
}
void DarkMage::OnCollision(ShapeComponent*, ShapeComponent*, Component*) {}

void DarkMage::SpawnMinion()
{
    if (!GetParent() || dead || !HasTarget() || summonsRemaining <= 0) return;
    const Rect body = hitbox->GetWorldBounds();
    auto skeleton = Unique<Skeleton>(Vector(body.x, body.y+body.h-48.f));
    skeleton->target = target;
    GetParent()->QueueAdd(std::move(skeleton));
    --summonsRemaining;
    maxMinions = summonsRemaining == 0;
}

void DarkMage::ClearAttacks()
{
    // Existing and queued heads keep weak references to the previous volley.
    headVolley = Shared<HeadVolley>();
    casting = healing = false;
}

size_t DarkMage::ActiveHeads() const
{
    return headVolley->activeHeads;
}

void DarkMage::FireHead()
{
    if (!GetParent() || !HasTarget() || dead || ActiveHeads() >= 6) return;
    FaceTarget();
    const Rect body = hitbox->GetWorldBounds();
    const Vector spawn(body.x+body.w*0.5f, body.y+body.h*0.25f);
    GetParent()->QueueAdd(Unique<Head>(spawn, target, Enraged() ? 260.f : 220.f, headVolley));
}
