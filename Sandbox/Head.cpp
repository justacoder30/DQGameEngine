#include "Head.h"
#include "Player.h"
#include <algorithm>
#include <cmath>

namespace {
    Vector Center(const Rect& r) { 
        return Vector(r.x + r.w * 0.5f, r.y + r.h * 0.5f); 
    }
    // Sweep a head's center against a rectangle expanded by the head radius.
    // This avoids tunnelling and resolves the earliest obstacle along the path.
    float Sweep(Vector from, Vector to, Rect r) {
        constexpr float radius = 9.f;
        r.x -= radius; r.y -= radius; r.w += radius * 2; r.h += radius * 2;
        float entry = 0.f, exit = 1.f;
        const float starts[] = {from.x, from.y}, deltas[] = {to.x-from.x, to.y-from.y};
        const float lo[] = {r.x, r.y}, hi[] = {r.x+r.w, r.y+r.h};
        for (int axis = 0; axis < 2; ++axis) {
            if (std::abs(deltas[axis]) < 0.0001f) {
                if (starts[axis] < lo[axis] || starts[axis] > hi[axis]) return 2.f;
            } else {
                float a = (lo[axis]-starts[axis])/deltas[axis];
                float b = (hi[axis]-starts[axis])/deltas[axis];
                if (a > b) std::swap(a,b);
                entry = std::max(entry,a); exit = std::min(exit,b);
                if (entry > exit) return 2.f;
            }
        }
        return entry;
    }
}


Head::Head(const Vector& spawnPosition, Player* player, float speed,
           const std::shared_ptr<HeadVolley>& group)
    : SpriteComponent("resource/img/Enemy/DarkMage/Head.png"), target(player), volley(group)
{
    position = spawnPosition;
    size = Vector(24.f, 24.f);
    anchor = Vector(0.5f, 0.5f);
    if (group) ++group->activeHeads;
    if (target) {
        const Vector aim = Center((target->hitbox->active ? target->hitbox : target->rollHitbox)->GetWorldBounds());
        const float dx = aim.x-position.x, dy = aim.y-position.y;
        const float length = std::max(1.f, std::sqrt(dx*dx+dy*dy));
        velocity = Vector(dx/length*speed, dy/length*speed);
    }
}

Head::~Head()
{
    if (auto group = volley.lock()) --group->activeHeads;
}

void Head::OnAttach()
{
    SpriteComponent::OnAttach();
    // A queued head can be drawn before its first update.
    flip = velocity.x > 0 ? Flip::Horizontal : Flip::None;
    SpriteComponent::OnUpdate(0.f);
}

bool Head::HasLiveTarget() const
{
    if (!GetParent()) return false;
    // Check scene membership before dereferencing a potentially removed player.
    for (const auto& owned : GetParent()->GetChildren())
        if (owned.get() == target) return !owned->IsRemovalPending() && target->hp > 0;
    return false;
}

void Head::HitPlayer(HeadVolley& group)
{
    if (target->invincible || target->hp <= 0 || group.hitGrace > 0) return;
    target->hp = std::max(0, target->hp-18);
    if (target->healthbar) target->healthbar->SetHealth(target->hp, target->MaxHP);
    target->state->ChangeState(target->hp == 0 ?
        static_cast<IState*>(target->deathState) : static_cast<IState*>(target->hurtState));
    group.hitGrace = 0.65f;
    Time::Freeze(0.025f);
    if (auto* camera = Renderer2D::GetCamera()) camera->Shake(1.f, 0.1f);
}

void Head::OnUpdate(float dt)
{
    auto group = volley.lock();
    if (!group || !HasLiveTarget()) { RemoveFromParent(); return; }
    age += dt;
    if (age > 4.f) { RemoveFromParent(); return; }

    // Brief tracking, followed by committed flight so the player can dodge.
    if (age < 0.7f) {
        const Vector aim = Center((target->hitbox->active ? target->hitbox : target->rollHitbox)->GetWorldBounds());
        const float dx = aim.x-position.x, dy = aim.y-position.y;
        const float length = std::max(1.f, std::sqrt(dx*dx+dy*dy));
        const float speed = std::sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
        const float blend = std::min(1.f, dt*1.6f);
        velocity.x += (dx/length*speed-velocity.x)*blend;
        velocity.y += (dy/length*speed-velocity.y)*blend;
    }

    const Vector next(position.x+velocity.x*dt, position.y+velocity.y*dt);
    float obstacle = 2.f;
    for (auto* shape : GameApp::GetBoardphase()->GetColliders()) {
        if (!shape->active) continue;
        const bool block = shape->layer == Layer::Ground ||
            (shape->layer == Layer::Attack && shape->GetParent() == target);
        auto* rect = dynamic_cast<RectangleComponent*>(shape);
        if (block && rect) obstacle = std::min(obstacle, Sweep(position,next,rect->GetWorldBounds()));
    }
    const float playerHit = Sweep(position,next,
        (target->hitbox->active ? target->hitbox : target->rollHitbox)->GetWorldBounds());
    if (playerHit <= 1.f && playerHit < obstacle) {
        HitPlayer(*group);
        RemoveFromParent();
        return;
    }
    if (obstacle <= 1.f) { RemoveFromParent(); return; }
    position = next;
    flip = velocity.x > 0 ? Flip::Horizontal : Flip::None;
    SpriteComponent::OnUpdate(dt);
}

void Head::OnDraw()
{
    if (!volley.expired() && HasLiveTarget()) SpriteComponent::OnDraw();
}
