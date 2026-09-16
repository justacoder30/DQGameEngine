#include "pch.h"
#include "SpatialGrid.h"

#include "Rect.h"
#include "Components/RectangleComponent.h"

#include <cmath>

void SpatialGrid::Init(float size)
{
    cellSize = size;

    grid.reserve(1024);
    queryStamp.reserve(512);
    queryResult.reserve(64);
}

void SpatialGrid::Clear()
{
    ++currentFrame;

    // Extremely unlikely, but protects against overflow.
    if (currentFrame == 0) {
        grid.clear();
        currentFrame = 1;
    }
}

int SpatialGrid::ToCell(float value) const
{
    return static_cast<int>( std::floor(value / cellSize) );
}

void SpatialGrid::Insert(ShapeComponent* obj)
{
    if (!obj) return;
    if (!obj->active) return;
    if (obj->shapeType != ShapeType::Rectangle) return;

    auto* rect = static_cast<RectangleComponent*>(obj);

    const Rect bounds = rect->GetWorldBounds();

    const int minX = ToCell(bounds.x);
    const int minY = ToCell(bounds.y);
    const int maxX = ToCell(bounds.x + bounds.w);
    const int maxY = ToCell(bounds.y + bounds.h);

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            GridCell& cell = grid[Hash(x, y)];

            if (cell.frame != currentFrame)
            {
                cell.objects.clear();
                cell.frame = currentFrame;
            }

            cell.objects.push_back(obj);
        }
    }
}

std::vector<ShapeComponent*> SpatialGrid::Query(ShapeComponent* obj)
{
    queryResult.clear();

    if (!obj) return queryResult;

    if (obj->shapeType != ShapeType::Rectangle)
        return queryResult;

    auto* rect = static_cast<RectangleComponent*>(obj);

    const Rect bounds = rect->GetWorldBounds();

    const int minX = ToCell(bounds.x);
    const int minY = ToCell(bounds.y);
    const int maxX = ToCell(bounds.x + bounds.w);
    const int maxY = ToCell(bounds.y + bounds.h);

    ++currentQuery;

    if (currentQuery == 0)
    {
        queryStamp.clear();
        currentQuery = 1;
    }

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            auto it = grid.find(Hash(x, y));

            if (it == grid.end()) continue;

            GridCell& cell = it->second;

            // Cell contains old data.
            if (cell.frame != currentFrame) continue;

            for (ShapeComponent* other : cell.objects)
            {
                if (other == obj) continue;

                auto stampIt = queryStamp.find(other);

                if (stampIt != queryStamp.end() && stampIt->second == currentQuery)
                    continue;

                queryStamp[other] = currentQuery;
                queryResult.push_back(other);
            }
        }
    }

    return queryResult;
}

std::uint64_t SpatialGrid::Hash(int x, int y) const
{
    return ((static_cast<std::uint64_t>(x) << 32) | static_cast<std::uint64_t>(y));
}