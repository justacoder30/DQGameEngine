#pragma once

#include "Components/ShapeComponent.h"
#include "Core/Board.h"

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace dqengine {

struct GridCell
{
    std::vector<ShapeComponent*> objects;
    std::uint64_t frame = 0;
};

class SpatialGrid : public Board
{
public:
    void Init(float size);
    void Clear() override;
    void Remove(ShapeComponent* obj) override;
    void Insert(ShapeComponent* obj) override;

    using Board::Query;
    void Query(ShapeComponent* obj, std::vector<ShapeComponent*>& result) override;

private:
    float cellSize = 256.0f;

    std::uint64_t currentFrame = 1;
    std::uint64_t currentQuery = 1;
    std::unordered_map<std::uint64_t, GridCell> grid;
    std::unordered_map<ShapeComponent*, std::uint64_t> queryStamp;


private:
    std::uint64_t Hash(int x, int y) const;
    int ToCell(float value) const;
};
} // namespace dqengine
