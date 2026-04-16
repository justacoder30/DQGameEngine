#pragma once
#include "Components/ShapeComponent.h"
#include <Core/Board.h>

struct GridCell
{
    std::vector<ShapeComponent*> objects;
};

class SpatialGrid : public Board
{
public:
    float cellSize = 256.f;

    std::unordered_map<long long, GridCell> grid;

	void Init(float size);
    void Clear() override;
    void Insert(ShapeComponent* obj) override;
    std::vector<ShapeComponent*> Query(ShapeComponent* obj) override;

private:
    long long Hash(int x, int y);
};

