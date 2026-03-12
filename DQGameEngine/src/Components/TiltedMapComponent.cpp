#include "pch.h"
#include "Components/TiltedMapComponent.h"
#include "Renderer/Renderer2D.h"
#include <filesystem>

TiltedMapComponent::TiltedMapComponent(const std::string& path)
{
    Load(path);
    BuildTiles();
}

void TiltedMapComponent::Load(const std::string& path)
{
    if (!std::filesystem::exists(path))
    {
        std::cout << "Map file NOT FOUND: " << path << std::endl;
        return;
    } else {
        std::cout << "Loading map: " << path << std::endl;
	}

    if (!m_Map.load(path))
    {
        std::cout << "Failed to load map: " << path << std::endl;
        return;
    }

    const auto& tileSets = m_Map.getTilesets();

    m_Textures.reserve(tileSets.size());

    for (auto& ts : tileSets)
    {
        m_Textures.emplace_back(ts.getImagePath());
    }

    m_TileSize = m_Map.getTileSize().x;

    auto mapSize = m_Map.getTileCount();

    m_Width = mapSize.x * m_TileSize;
    m_Height = mapSize.y * m_TileSize;
}

void TiltedMapComponent::BuildTiles()
{
    const auto& layers = m_Map.getLayers();
    const auto& tileSets = m_Map.getTilesets();

    for (auto& layer : layers)
    {
        if (layer->getType() != tmx::Layer::Type::Tile)
            continue;

        auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        auto& tiles = tileLayer.getTiles();

        auto mapSize = m_Map.getTileCount();

        for (uint32_t y = 0; y < mapSize.y; y++)
        {
            for (uint32_t x = 0; x < mapSize.x; x++)
            {
                uint32_t index = y * mapSize.x + x;

                if (index >= tiles.size())
                    continue;

                auto tile = tiles[index];

                if (tile.ID == 0)
                    continue;

                for (size_t i = 0; i < tileSets.size(); i++)
                {
                    auto& ts = tileSets[i];

                    if (tile.ID < ts.getFirstGID() ||
                        tile.ID >= ts.getFirstGID() + ts.getTileCount())
                        continue;

                    int localID = tile.ID - ts.getFirstGID();

                    Texture& tex = m_Textures[i];

                    int tileCountX = tex.GetWidth() / m_TileSize;

                    Rect src;

                    src.w = m_TileSize;
                    src.h = m_TileSize;

                    src.x = (localID % tileCountX) * m_TileSize;
                    src.y = (localID / tileCountX) * m_TileSize;

                    Rect dst;

                    dst.x = x * m_TileSize;
                    dst.y = y * m_TileSize;
                    dst.w = m_TileSize;
                    dst.h = m_TileSize;

                    Tile t;

                    t.texture = &tex;
                    t.src = src;
                    t.dst = dst;

                    m_Tiles.push_back(t);

                    break;
                }
            }
        }
    }
}

void TiltedMapComponent::OnDraw()
{
    for (auto& tile : m_Tiles)
    {
        Renderer2D::Draw(
            *tile.texture,
            tile.src,
            tile.dst,
            tile.flip,
            tile.rotation
        );
    }
}
