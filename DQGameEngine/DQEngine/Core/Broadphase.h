#pragma once

#include "Components/ShapeComponent.h"
#include "Board.h"
#include "CollisionCallbacks.h"

#include <functional>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

using CollisionPair = std::pair<ShapeComponent*, ShapeComponent*>;

struct CollisionPairHash
{
    std::size_t operator()(const CollisionPair& pair) const noexcept
    {
        const std::size_t h1 = std::hash<ShapeComponent*>{}(pair.first);

        const std::size_t h2 = std::hash<ShapeComponent*>{}(pair.second);

        return h1 ^ (h2 << 1);
    }
};

class Boardphase
{
public:
    Boardphase();

    void Add(ShapeComponent* c);
    void Remove(ShapeComponent* c);

    void Run();

    void HandleCollision(ShapeComponent* a, ShapeComponent* b);
    void HandleCollisionStart(ShapeComponent* a, ShapeComponent* b);
    void HandleCollisionEnd(ShapeComponent* a,  ShapeComponent* b);

    const std::vector<ShapeComponent*>& GetColliders() const { return colliders; }

    Board* GetBoard() { return board.get(); }

    void SetBoard(std::unique_ptr<Board> b) { board = std::move(b); }

private:
    CollisionPair MakePair( ShapeComponent* a, ShapeComponent* b) const;

private:
    std::vector<ShapeComponent*> colliders;
    std::unordered_set<CollisionPair, CollisionPairHash> currentCollisions;
    std::unordered_set<CollisionPair, CollisionPairHash> previousCollisions;
    std::unique_ptr<Board> board;
};

bool ShouldCollide( ShapeComponent* a, ShapeComponent* b);