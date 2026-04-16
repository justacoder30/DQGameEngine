#include "pch.h"
#include "SpatialGrid.h"
#include "Rect.h"   
#include "Components/RectangleComponent.h"

void SpatialGrid::Init(float size)
{
    cellSize = size;
}

void SpatialGrid::Clear()
{
	grid.clear();
}

void SpatialGrid::Insert(ShapeComponent* obj)
{
	if (obj->shapeType != ShapeType::Rectangle) return;
	auto r = static_cast<RectangleComponent*>(obj);    
    Rect bounds = r->GetWorldBounds();

    int minX = (int)(bounds.x / cellSize);
    int minY = (int)(bounds.y / cellSize);
    int maxX = (int)((bounds.x + bounds.w) / cellSize);
    int maxY = (int)((bounds.y + bounds.h) / cellSize);

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            grid[Hash(x, y)].objects.push_back(obj);
        }
    }
}

std::vector<ShapeComponent*> SpatialGrid::Query(ShapeComponent* obj)
{
    std::vector<ShapeComponent*> result;

    if (obj->shapeType != ShapeType::Rectangle) return result;
    auto r = static_cast<RectangleComponent*>(obj);
    Rect bounds = r->GetWorldBounds();

    int minX = (int)(bounds.x / cellSize);
    int minY = (int)(bounds.y / cellSize);
    int maxX = (int)((bounds.x + bounds.w) / cellSize);
    int maxY = (int)((bounds.y + bounds.h) / cellSize);

    std::unordered_set<ShapeComponent*> unique;

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            auto it = grid.find(Hash(x, y));
            if (it == grid.end()) continue;

            for (auto* other : it->second.objects)
            {
                if (other != obj)
                    unique.insert(other);
            }
        }
    }

    result.assign(unique.begin(), unique.end());
    return result;
}

long long SpatialGrid::Hash(int x, int y)
{
	return ((long long)x << 32) | (unsigned int)y;
}
