#pragma once
#include "Engine.h"
#include <memory>

class Player;

// Shared only by one boss's projectiles. Dropping it cancels that volley,
// including heads that are still queued, without storing a raw boss pointer.
struct HeadVolley
{
    size_t activeHeads = 0;
    float hitGrace = 0.f;
};

class Head : public SpriteComponent
{
public:
    Head(const Vector& spawnPosition, Player* target, float speed,
         const std::shared_ptr<HeadVolley>& volley);
    ~Head() override;
    void OnUpdate(float dt) override;
    void OnDraw() override;

protected:
    void OnAttach() override;

private:
    Player* target;
    std::weak_ptr<HeadVolley> volley;
    Vector velocity;
    float age = 0.f;
    bool HasLiveTarget() const;
    void HitPlayer(HeadVolley& group);
};
